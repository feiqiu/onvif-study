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

void cb_discovery(char *DeviceXAddr)
{
    struct tagCapabilities capa;

    ONVIF_GetCapabilities(DeviceXAddr, &capa);
}

int main(int argc, char **argv)
{
    ONVIF_DetectDevice(cb_discovery);

    return 0;
}