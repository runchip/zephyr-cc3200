#ifdef CONFIG_CC3200SDK_SIMPLELINK
#include <kernel.h>
#include <misc/printk.h>
#include <simplelink.h>
#define printf printk
#define UART_PRINT printk
#define perror(str) printk("perror %s+%d: %s", __FILE__, __LINE__, str)
#else // CONFIG_CC3200SDK_SIMPLELINK

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#endif // CONFIG_CC3200SDK_SIMPLELINK

#define HOST "example.com"
#define PORT 80

const char REQUEST[] = "GET / HTTP/1.1\r\n"
		       "Host: " HOST "\r\n"
		       "User-Agent: curl/7.47.0\r\n"
		       "Accept: */*\r\n"
		       "\r\n";
char response[1024];

static void wlan_init(void);

uint32_t resolve_host_addr(const char *host)
{
	uint32_t dest = 0;
#ifdef CONFIG_CC3200SDK_SIMPLELINK
	sl_NetAppDnsGetHostByName((_i8 *)host, strlen(host), (_u32 *)&dest,
				  SL_AF_INET);
	dest = htonl(dest);
#else  // CONFIG_CC3200SDK_SIMPLELINK
	struct hostent *he;

	he = gethostbyname(host);
	if (he && he->h_addr_list && he->h_addr_list[0]) {
		dest = ((struct in_addr *)(he->h_addr_list[0]))->s_addr;
	}
#endif // CONFIG_CC3200SDK_SIMPLELINK
	return dest;
}

int main(int argc, char *argv[])
{
	int sock, ret;
	struct sockaddr_in server_addr;

	wlan_init();

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("socket");
	}

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = resolve_host_addr(HOST);

#if !defined(CONFIG_CC3200SDK_SIMPLELINK)
	inet_ntop(AF_INET, &server_addr.sin_addr, response, sizeof(response));
	printf("server: %s\n", response);
#endif

	printf("connect to server...\n");
	ret = connect(sock, (const struct sockaddr *)&server_addr,
		      sizeof(server_addr));
	if (ret < 0) {
		perror("connect");
	}

	printf("send request...\n");
	ret = send(sock, REQUEST, strlen(REQUEST), 0);
	if (ret < 0) {
		perror("send");
	}
	printf("%d bytes sent\n", ret);

	printf("recv response...\n");
	while ((ret = recv(sock, response, sizeof(response), 0)) > 0) {
		printf("===\n%s---\n", response);
	}
	printf("response: %s\n", response);
	if (ret < 0) {
		perror("recv");
	}

	close(sock);

	return 0;
}

#ifdef CONFIG_CC3200SDK_SIMPLELINK

#if !defined(AP_SSID) || !defined(AP_PSK)
#error AP_SSID or AP_PSK not defined
#endif

#define SSID_NAME AP_SSID
#define SECURITY_KEY AP_PSK
#define SECURITY_TYPE SL_SEC_TYPE_WPA_WPA2

#define CLR_STATUS_BIT_ALL(status_variable) (status_variable = 0)
#define SET_STATUS_BIT(status_variable, bit) (status_variable |= (1 << (bit)))
#define CLR_STATUS_BIT(status_variable, bit) (status_variable &= ~(1 << (bit)))
#define GET_STATUS_BIT(status_variable, bit) (status_variable & (1 << (bit)))

#define SSID_LEN_MAX 32
unsigned long g_wlan_status;
unsigned char g_connected_ssid[SSID_LEN_MAX + 1];
unsigned char g_connected_bssid[SL_BSSID_LENGTH];

enum {
	STATUS_BIT_NWP_INIT = 0,
	STATUS_BIT_CONNECTION,
};

void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
	static slWlanConnectAsyncResponse_t *pEventData;

	if (pWlanEvent == NULL) {
		return;
	}

	switch (pWlanEvent->Event) {
	case SL_WLAN_CONNECT_EVENT: {
		SET_STATUS_BIT(g_wlan_status, STATUS_BIT_CONNECTION);
		pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;

		// Copy new connection SSID and BSSID to global parameters
		memcpy(g_connected_ssid, pEventData->ssid_name,
		       pEventData->ssid_len);
		memcpy(g_connected_bssid, pEventData->bssid, SL_BSSID_LENGTH);

		UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s ,"
			   " BSSID: %x:%x:%x:%x:%x:%x\n",
			   g_connected_ssid, g_connected_bssid[0],
			   g_connected_bssid[1], g_connected_bssid[2],
			   g_connected_bssid[3], g_connected_bssid[4],
			   g_connected_bssid[5]);
	} break;

	case SL_WLAN_DISCONNECT_EVENT: {
		CLR_STATUS_BIT(g_wlan_status, STATUS_BIT_CONNECTION);
		pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

		// If the user has initiated 'Disconnect' request,
		//'reason_code' is
		// SL_WLAN_DISCONNECT_USER_INITIATED_DISCONNECTION
		if (pEventData->reason_code ==
		    SL_WLAN_DISCONNECT_USER_INITIATED_DISCONNECTION) {
			UART_PRINT(
			    "[WLAN EVENT]Device disconnected from the AP: %s,"
			    "BSSID: %x:%x:%x:%x:%x:%x on application's "
			    "request\n\r",
			    g_connected_ssid, g_connected_bssid[0],
			    g_connected_bssid[1], g_connected_bssid[2],
			    g_connected_bssid[3], g_connected_bssid[4],
			    g_connected_bssid[5]);
		} else {
			UART_PRINT(
			    "[WLAN ERROR]Device disconnected from the AP AP: "
			    "%s,"
			    "BSSID: %x:%x:%x:%x:%x:%x on an ERROR..!!\n\r",
			    g_connected_ssid, g_connected_bssid[0],
			    g_connected_bssid[1], g_connected_bssid[2],
			    g_connected_bssid[3], g_connected_bssid[4],
			    g_connected_bssid[5]);
		}
		memset(g_connected_ssid, 0, sizeof(g_connected_ssid));
		memset(g_connected_bssid, 0, sizeof(g_connected_bssid));
	} break;

	default: {
		UART_PRINT("[WLAN EVENT] Unexpected event [0x%lx]\n\r",
			   pWlanEvent->Event);
	} break;
	}
}

#define WLAN_STACK_SIZE 4096
char wlan_thread_stack[WLAN_STACK_SIZE];
atomic_t wlan_thread_started;
void wlan_thread_work(void *arg1, void *arg2, void *arg3)
{
	printf("%s start\n", __func__);
	while (1) {
		atomic_set(&wlan_thread_started, 1);
		_SlNonOsMainLoopTask();
	}
}

void wlan_init(void)
{
	int mode = 0;
	static SlSecParams_t security_params = {0};

	k_thread_spawn(wlan_thread_stack, WLAN_STACK_SIZE, wlan_thread_work,
		       NULL, NULL, NULL, K_PRIO_PREEMPT(1), 0, K_NO_WAIT);
	while (atomic_get(&wlan_thread_started) == 0) {
		k_sleep(100);
	}

#define EXPECT_OK(expr)                                                        \
	({                                                                     \
		long ret = (expr);                                             \
		printf(#expr ": %s\n", (ret < 0) ? "FAIL" : "OK");             \
		ret;                                                           \
	})

	printf("wlan start\n");
	mode = EXPECT_OK(sl_Start(NULL, NULL, NULL));
	if (mode != ROLE_STA) {
		printf("wlan stop\n");
		EXPECT_OK(sl_Stop(200));
		printf("set mode to STA\n");
		EXPECT_OK(sl_WlanSetMode(ROLE_STA));
		printf("wlan start\n");
		EXPECT_OK(sl_Start(NULL, NULL, NULL));
	}
	printf("set mode successed\n");

	// AUTO policy
	EXPECT_OK(sl_WlanPolicySet(SL_POLICY_CONNECTION,
				   SL_CONNECTION_POLICY(1, 0, 0, 0, 0), NULL,
				   0));

	security_params.Key = (signed char *)SECURITY_KEY;
	security_params.KeyLen = strlen(SECURITY_KEY);
	security_params.Type = SECURITY_TYPE;
	EXPECT_OK(sl_WlanProfileAdd(SSID_NAME, strlen(SSID_NAME), NULL,
				    &security_params, NULL, 1, 0));

	printf("try to connect to AP %s %s\n", SSID_NAME, SECURITY_KEY);
	EXPECT_OK(sl_WlanConnect(SSID_NAME, strlen(SSID_NAME), NULL,
				 &security_params, NULL));
	while (GET_STATUS_BIT(g_wlan_status, STATUS_BIT_CONNECTION) == 0) {
		printf("connecting...\n");
		k_sleep(1000);
	}
}

#else  // CONFIG_CC3200SDK_SIMPLELINK
void wlan_init(void) {}
#endif // CONFIG_CC3200SDK_SIMPLELINK
