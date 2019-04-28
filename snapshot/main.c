/************************************************************************
**
** ���ߣ�����ƺ
** ���ڣ�2017-05-03
** ���ͣ�http://blog.csdn.net/benkaoya
** ��������ȡIPC����Ƶ������ʾ������
**
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "onvif_comm.h"
#include "onvif_dump.h"

/************************************************************************
**������ONVIF_GetSnapshotUri
**���ܣ���ȡ�豸ͼ��ץ�ĵ�ַ(HTTP)
**������
        [in]  MediaXAddr    - ý������ַ
        [in]  ProfileToken  - the media profile token
        [out] uri           - ���صĵ�ַ
        [in]  sizeuri       - ��ַ�����С
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
    1). �������е�ProfileToken��֧��ͼ��ץ�ĵ�ַ��������XXXƷ�Ƶ�IPC��������������
    profile0/profile1/TestMediaProfile������TestMediaProfile���ص�ͼ��ץ�ĵ�ַ����
    ��ָ�롣
************************************************************************/
int ONVIF_GetSnapshotUri(const char *MediaXAddr, char *ProfileToken, char *uri, unsigned int sizeuri)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _trt__GetSnapshotUri         req;
    struct _trt__GetSnapshotUriResponse rep;

    SOAP_ASSERT(NULL != MediaXAddr);
    SOAP_ASSERT(NULL != uri);
    memset(uri, 0x00, sizeuri);

    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    req.ProfileToken = ProfileToken;
    result = soap_call___trt__GetSnapshotUri(soap, MediaXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetSnapshotUri");

    dump_trt__GetSnapshotUriResponse(&rep);

    result = -1;
    if (NULL != rep.MediaUri) {
        if (NULL != rep.MediaUri->Uri) {
            if (sizeuri > strlen(rep.MediaUri->Uri)) {
                strcpy(uri, rep.MediaUri->Uri);
                result = 0;
            } else {
                SOAP_DBGERR("Not enough cache!\n");
            }
        }
    }

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

void cb_discovery(char *DeviceXAddr)
{
    int stmno = 0;                                                              // ������ţ�0Ϊ��������1Ϊ������
    int profile_cnt = 0;                                                        // �豸�����ļ�����
    struct tagProfile *profiles = NULL;                                         // �豸�����ļ��б�
    struct tagCapabilities capa;                                                // �豸������Ϣ

    char cmd[256];
    char uri[ONVIF_ADDRESS_SIZE] = {0};                                         // ������֤��Ϣ��URI��ַ
    char uri_auth[ONVIF_ADDRESS_SIZE + 50] = {0};                               // ������֤��Ϣ��URI��ַ

    ONVIF_GetCapabilities(DeviceXAddr, &capa);                                  // ��ȡ�豸������Ϣ����ȡý������ַ��
    
    profile_cnt = ONVIF_GetProfiles(capa.MediaXAddr, &profiles);                // ��ȡý��������Ϣ����/������������Ϣ��

    if (profile_cnt > stmno) {
        ONVIF_GetSnapshotUri(capa.MediaXAddr, profiles[stmno].token, uri, sizeof(uri)); // ��ȡͼ��ץ��URI

        make_uri_withauth(uri, USERNAME, PASSWORD, uri_auth, sizeof(uri_auth)); // ���ɴ���֤��Ϣ��URI���е�IPCҪ����֤��

        sprintf(cmd, "wget -O out.jpeg '%s'", uri_auth);                        // ʹ��wget����ͼƬ
        system(cmd);
    }

    if (NULL != profiles) {
        free(profiles);
        profiles = NULL;
    }
}

int main(int argc, char **argv)
{
    ONVIF_DetectDevice(cb_discovery);

    return 0;
}