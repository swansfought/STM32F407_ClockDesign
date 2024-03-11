- [STM32F407_ClockDesign](#stm32f407-clockdesign)
- [应用技术](#----)
- [项目描述](#----)
- [主要工作](#----)

## STM32F407_ClockDesign
  此项目是基于 STM32F407ZG6 开发板实现的一个多功能 触摸 电子时钟 用户通过窗口界面进行人机交互。
  
  示例图在【_部分截图】文件夹中，感兴趣就去看看。

## 应用技术
  STM32F407 、 C 、 emWin 、 UcosIII 、 RTC 。

  
## 主要工作
  1、将 UCOSII 移植到 STM32F407 上 ，设置相应任务 并启动；
  
  2、界面使用 GUIBuilder 设置交互界面并生成 C 代码；
  
  3、实现时间、日历、倒计时、闹钟以及温度等功能 。
  
## 功能模块（图片形式展示）
### 1. 主界面

![image](https://github.com/swansfought/STM32F407_ClockDesign/blob/main/_%E9%83%A8%E5%88%86%E6%88%AA%E5%9B%BE/%E4%B8%BB%E7%95%8C%E9%9D%A2.jpg)

### 2. 添加闹钟

![image](https://github.com/swansfought/STM32F407_ClockDesign/blob/main/_%E9%83%A8%E5%88%86%E6%88%AA%E5%9B%BE/%E6%B7%BB%E5%8A%A0%E9%97%B9%E9%92%9F.jpg)

### 3.设计倒计时

![image](https://github.com/swansfought/STM32F407_ClockDesign/blob/main/_%E9%83%A8%E5%88%86%E6%88%AA%E5%9B%BE/%E8%AE%BE%E7%BD%AE%E5%80%92%E8%AE%A1%E6%97%B6.jpg)

### 4.设置日历

![image](https://github.com/swansfought/STM32F407_ClockDesign/blob/main/_%E9%83%A8%E5%88%86%E6%88%AA%E5%9B%BE/%E8%AE%BE%E7%BD%AE%E6%97%A5%E5%8E%86.jpg)

### 5.设置日期&时间

![image](https://github.com/swansfought/STM32F407_ClockDesign/blob/main/_%E9%83%A8%E5%88%86%E6%88%AA%E5%9B%BE/%E8%AE%BE%E7%BD%AE%E6%97%A5%E6%9C%9F%26%E6%97%B6%E9%97%B4.jpg)

### 6.闹钟提示

![image](https://github.com/swansfought/STM32F407_ClockDesign/blob/main/_%E9%83%A8%E5%88%86%E6%88%AA%E5%9B%BE/%E9%97%B9%E9%92%9F%E6%8F%90%E7%A4%BA.jpg)







