import machine
import time
import ujson
from umqtt.simple import MQTTClient

# WiFi连接信息
WIFI_SSID = 'wj'
WIFI_PASSWORD = '15998907181'

# MQTT连接信息
MQTT_SERVER = 'broker.mqttdashboard.com'
MQTT_TOPIC_SEND = 'TEST1234'
MQTT_TOPIC_RECEIVE = 'TEST12345'

# 初始化串口
uart = machine.UART(1, baudrate=115200, tx=0, rx=1)

# 连接WiFi
import network
sta_if = network.WLAN(network.STA_IF)
sta_if.active(True)
sta_if.connect(WIFI_SSID, WIFI_PASSWORD)
while not sta_if.isconnected():
    pass
print('WiFi已连接')

# 连接到MQTT服务器
client = MQTTClient("esp32", MQTT_SERVER)
client.connect()
print('已连接到MQTT服务器')

def mqtt_callback(topic, msg):
    if topic == MQTT_TOPIC_RECEIVE.encode('utf-8'):  # 将字符串转换为字节串
        try:
            print(msg)
            #data = ujson.loads(msg.decode('utf-8'))  # 将字节串解码为字符串，并解析为JSON格式
            uart.write(msg)  # 将字符串转换为字节串，并发送到串口1
            print('已发送接收到的数据到串口1:', data)
        except Exception as e:
            print('解析JSON数据时出错:', e)

client.set_callback(mqtt_callback)
client.subscribe(MQTT_TOPIC_RECEIVE)

while True:
    if uart.any():  # 检查是否有数据可读
        data = uart.read()  # 读取串口数据
        print('接收到的数据:', data)

        try:
            json_data = ujson.loads(data)  # 解析JSON格式数据
            adc_value = json_data.get('adc', None)  # 获取键为'adc'的值

            if adc_value is not None:
                print('提取到的ADC值:', adc_value)
                client.publish(MQTT_TOPIC_SEND, ujson.dumps({'adc': adc_value}))  # 发布ADC值到MQTT主题
                print('已发布ADC值到MQTT主题')
        except Exception as e:
            print('解析JSON数据时出错:', e)
    
    client.check_msg()  # 检查是否有新的MQTT消息
    time.sleep(0.1)  # 等待一段时间后再次检查是否有数据