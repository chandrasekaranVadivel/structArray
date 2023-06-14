#include <Arduino_FreeRTOS.h>
#include <queue.h>
typedef struct adc_h
{
  int pin[2];
  int readValue[2];
}adc_t;
QueueHandle_t printQueue;
TaskHandle_t *adcTask_h,*printTask_h,*ledTask_h;
void adcTask(void *prm)
{
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  adc_t *adc=(adc_t*)malloc(sizeof(adc_t));
  while(1)
  {
    adc->pin[0]=0;
    adc->readValue[0]=analogRead(A0);
    adc->pin[1]=1;
    adc->readValue[1]=analogRead(A1);
    xQueueSend(printQueue, adc, portMAX_DELAY);
    vTaskDelay(100);
  }
}
void printTask(void *prm)
{
  char arr[20];
  adc_t *adc_r;//=(adc_t*)malloc(sizeof(adc_t));
  while(1)
  {
    if(xQueueReceive(printQueue, adc_r, pdMS_TO_TICKS(10))==pdTRUE)
    {
    sprintf(arr,"%s%d=%d,%s%d=%d\n","pin",adc_r->pin[0],adc_r->readValue[0],"pin",adc_r->pin[1],adc_r->readValue[1]);
    Serial.print(arr);
 //   vTaskDelay(500);
    }
  }
}
void ledTask(void *prm)
{
  pinMode(LED_BUILTIN,OUTPUT);
  while(1)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    vTaskDelay(pdMS_TO_TICKS(100));
    digitalWrite(LED_BUILTIN,LOW);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
void setup() 
{
  Serial.begin(9600);
  Serial.println("welcome\n");
  printQueue=xQueueCreate(5,sizeof(adc_t));
  if(printQueue)
  {
    Serial.println("queue created success\n");
  }
  xTaskCreate(adcTask,"adcTask",125,NULL,1,adcTask_h);
  xTaskCreate(printTask,"printTask",125,NULL,1,printTask_h);
  xTaskCreate(ledTask,"ledTask",125,NULL,1,ledTask_h);
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

}
