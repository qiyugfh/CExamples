#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


#define MAX_BUF_LEN 2048


void write_log(FILE *fp, const char *format, ...){
  va_list args;
  va_start(args, format);
  vfprintf(fp, format, args);
  va_end(args);
}

void write_log2(FILE *fp, const char *format, ...){
  va_list args;
  va_start(args, format);
  int nwrite = vsnprintf(fp, MAX_BUF_LEN, format, args);
  if(nwrite < 0){
    printf("write error!\n");	
  }
  va_end(args);
}

int main(void){
  FILE *fp = NULL;
  if((fp = fopen("tnn.txt", "w+")) == NULL){
    return -1;
  }
    
  char val[MAX_BUF_LEN] = "{\"src_name\":\"192.168.2.192\", \"src_port\":\"51686\", \"dst_name\":\"101.226.211.216\", \"dst_port\":\"80\", \"ip_proto\":\"TCP\", \"proto\":\"7.48/HTTP.QQ\", \"cat\":\"Chat\", \"flow_size\":\"3 pkts/2336 bytes <-> 1 pkts/60 bytes\", \"user_agent\":\"Mozilla/5.0 (Linux; Android 7.0; EVA-AL00 Build/HUAWEIEVA-AL00; wv) AppleWebKit/537.36 (KHTML, like Gecko) Version/4.0 Chrome/57.0.2987.132 MQQBrowser/6.2 TBS/044109 Mobile Safari/537.36 MicroMessenger/6.6.6.1300(0x26060638) NetType/WIFI L, \"url\":\"http://btrace.qq.com/kvcollect?BossId=2973&Pwd=1557019983&step=3&vid=b0698xdvajo&platform=60301&loadwait=0&val=0&t=0.8031618402952863&url=http%3A%2F%2Fmp.weixin.qq.com%2Fs%3F__biz%3DMzUyODU5NTM0OQ%3D%3D%26mid%3D2247488501%26idx%3D1%26sn%3D49d16b086d1364530c8b7fc753a6f205%26chksm%3Dfa6cb30fcd1b3a1953e16480751089121171b1c6daf0ae17a8831a003812b36fc601d9e9bce3%26scene%3D0%26ascene%3D7%26devicetype%3Dandroid-24%26version%3D26060638%26nettype%3DWIFI%26abtest_cookie%3DBAABAAoACwAMABIACAA%252Bix4A44seAEOP}";
  printf("%s\n", val);
  write_log(fp, val);

  fclose(fp);
    
  return 0;
}



