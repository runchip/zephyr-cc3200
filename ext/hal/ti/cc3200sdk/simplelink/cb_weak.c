#include <simplelink.h>

#define WEAK __attribute__((weak))

WEAK void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
}

WEAK void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *slGeneralEvent)
{
}

WEAK void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
}

WEAK void SimpleLinkSockEventHandler(SlSockEvent_t* pSlSockEvent)
{
}

WEAK void SimpleLinkNetAppEventHandler(SlNetAppEvent_t* pSlNetApp)
{
}
