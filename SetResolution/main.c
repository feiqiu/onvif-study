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
**������ONVIF_GetVideoEncoderConfigurationOptions
**���ܣ���ȡָ����Ƶ���������õĲ���ѡ�
**������
        [in] MediaXAddr   - ý������ַ
        [in] ConfigurationToken - ��Ƶ���������õ������ַ��������ΪNULL�����ȡ������Ƶ���������õ�ѡ���������һ��
                                  �޷�����ѡ��ǹ����ĸ���Ƶ�����������ģ�
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
    1). �����ַ�ʽ���Ի�ȡָ����Ƶ���������õĲ���ѡ���һ���Ǹ���ConfigurationToken����һ��
        �Ǹ���ProfileToken
************************************************************************/
int ONVIF_GetVideoEncoderConfigurationOptions(const char *MediaXAddr, char *ConfigurationToken)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _trt__GetVideoEncoderConfigurationOptions          req;
    struct _trt__GetVideoEncoderConfigurationOptionsResponse  rep;

    SOAP_ASSERT(NULL != MediaXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    req.ConfigurationToken = ConfigurationToken;

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);
    result = soap_call___trt__GetVideoEncoderConfigurationOptions(soap, MediaXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetVideoEncoderConfigurationOptions");

    dump_trt__GetVideoEncoderConfigurationOptionsResponse(&rep);

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

/************************************************************************
**������ONVIF_GetVideoEncoderConfiguration
**���ܣ���ȡ�豸��ָ������Ƶ������������Ϣ
**������
        [in] MediaXAddr - ý������ַ
        [in] ConfigurationToken - ��Ƶ���������õ������ַ���
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
************************************************************************/
int ONVIF_GetVideoEncoderConfiguration(const char *MediaXAddr, char *ConfigurationToken)
{
    int result = 0;
    struct soap *soap = NULL;
    struct _trt__GetVideoEncoderConfiguration          req;
    struct _trt__GetVideoEncoderConfigurationResponse  rep;

    SOAP_ASSERT(NULL != MediaXAddr);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);

    memset(&req, 0x00, sizeof(req));
    memset(&rep, 0x00, sizeof(rep));
    req.ConfigurationToken = ConfigurationToken;
    result = soap_call___trt__GetVideoEncoderConfiguration(soap, MediaXAddr, NULL, &req, &rep);
    SOAP_CHECK_ERROR(result, soap, "GetVideoEncoderConfiguration");

    dump_trt__GetVideoEncoderConfigurationResponse(&rep);

EXIT:

    if (NULL != soap) {
        ONVIF_soap_delete(soap);
    }

    return result;
}

/************************************************************************
**������ONVIF_SetVideoEncoderConfiguration
**���ܣ��޸�ָ������Ƶ������������Ϣ
**������
        [in] MediaXAddr - ý������ַ
        [in] venc - ��Ƶ������������Ϣ
**���أ�
        0�����ɹ�����0����ʧ��
**��ע��
    1). �����õķֱ��ʱ�����GetVideoEncoderConfigurationOptions���صġ��ֱ���ѡ����е�һ�֣�
    �������SetVideoEncoderConfiguration��ʧ�ܡ�
************************************************************************/
int ONVIF_SetVideoEncoderConfiguration(const char *MediaXAddr, struct tagVideoEncoderConfiguration *venc)
{
    int result = 0;
    struct soap *soap = NULL;

    struct _trt__GetVideoEncoderConfiguration           gVECfg_req;
    struct _trt__GetVideoEncoderConfigurationResponse   gVECfg_rep;

    struct _trt__SetVideoEncoderConfiguration           sVECfg_req;
    struct _trt__SetVideoEncoderConfigurationResponse   sVECfg_rep;

    SOAP_ASSERT(NULL != MediaXAddr);
    SOAP_ASSERT(NULL != venc);
    SOAP_ASSERT(NULL != (soap = ONVIF_soap_new(SOAP_SOCK_TIMEOUT)));

    memset(&gVECfg_req, 0x00, sizeof(gVECfg_req));
    memset(&gVECfg_rep, 0x00, sizeof(gVECfg_rep));
    gVECfg_req.ConfigurationToken = venc->token;

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);
    result = soap_call___trt__GetVideoEncoderConfiguration(soap, MediaXAddr, NULL, &gVECfg_req, &gVECfg_rep);
    SOAP_CHECK_ERROR(result, soap, "GetVideoEncoderConfiguration");

    if (NULL == gVECfg_rep.Configuration) {
        SOAP_DBGERR("video encoder configuration is NULL.\n");
        goto EXIT;
    }

    memset(&sVECfg_req, 0x00, sizeof(sVECfg_req));
    memset(&sVECfg_rep, 0x00, sizeof(sVECfg_rep));

    sVECfg_req.ForcePersistence = xsd__boolean__true_;
    sVECfg_req.Configuration    = gVECfg_rep.Configuration;

    if (NULL != sVECfg_req.Configuration->Resolution) {
        sVECfg_req.Configuration->Resolution->Width  = venc->Width;
        sVECfg_req.Configuration->Resolution->Height = venc->Height;
    }

    ONVIF_SetAuthInfo(soap, USERNAME, PASSWORD);
    result = soap_call___trt__SetVideoEncoderConfiguration(soap, MediaXAddr, NULL, &sVECfg_req, &sVECfg_rep);
    SOAP_CHECK_ERROR(result, soap, "SetVideoEncoderConfiguration");

EXIT:

    if (SOAP_OK == result) {
        SOAP_DBGLOG("\nSetVideoEncoderConfiguration success!!!\n");
    }

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

    ONVIF_GetCapabilities(DeviceXAddr, &capa);                                  // ��ȡ�豸������Ϣ����ȡý������ַ��
    
    profile_cnt = ONVIF_GetProfiles(capa.MediaXAddr, &profiles);                // ��ȡý��������Ϣ����/������������Ϣ��

    if (profile_cnt > stmno) {
        struct tagVideoEncoderConfiguration venc;
        char *vencToken = profiles[stmno].venc.token;

        ONVIF_GetVideoEncoderConfigurationOptions(capa.MediaXAddr, vencToken);  // ��ȡ������֧�ֵ���Ƶ����������ѡ�

        ONVIF_GetVideoEncoderConfiguration(capa.MediaXAddr, vencToken);         // ��ȡ��������ǰ����Ƶ����������

        venc = profiles[stmno].venc;
        venc.Height = 960;
        venc.Width  = 1280;
        ONVIF_SetVideoEncoderConfiguration(capa.MediaXAddr, &venc);             // ���ø�������ǰ����Ƶ����������

        ONVIF_GetVideoEncoderConfiguration(capa.MediaXAddr, vencToken);         // �۲��Ƿ��޸ĳɹ�
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