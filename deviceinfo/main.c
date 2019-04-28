/************************************************************************
**
** ���ߣ�����ƺ
** ���ڣ�2017-05-03
** ���ͣ�http://blog.csdn.net/benkaoya
** ������IPC��ȡ�豸������Ϣʾ������
**
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "onvif_comm.h"
#include "onvif_dump.h"

/************************************************************************
**������ONVIF_GetDeviceInformation
**���ܣ���ȡ�豸������Ϣ
**������
        [in] DeviceXAddr - �豸�����ַ
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
************************************************************************/
int ONVIF_GetDeviceInformation(const char *DeviceXAddr)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _tds__GetDeviceInformation           req;
    struct _tds__GetDeviceInformationResponse   rep;

    SOAP_ASSERT(NULL != DeviceXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    result = soap_call___tds__GetDeviceInformation(soap, DeviceXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetDeviceInformation");

    dump_tds__GetDeviceInformationResponse(&rep);

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

void cb_discovery(char *DeviceXAddr)
{
    ONVIF_GetDeviceInformation(DeviceXAddr);
}

int main(int argc, char **argv)
{
    ONVIF_DetectDevice(cb_discovery);

    return 0;
}