## Дизайн пользовательского интерфейса
Содержит следующие элементы пользовательского интерфейса: 
* Заряд аккамулятора
* Кнопка

---
## Заряд аккамулятора
### Функции библиотеки battery.h
#### void batt_Init(t_batt *ptrbatt, void (*ptrdesign)(t_batt), int16_t x0, int16_t y0)  
- **Описание функции**  
Функция инициализирует параметры батареи и записывает их в структуру t_batt  
- **Параметры**  
**ptrbatt:** указатель на переменную типа t_batt  
**ptrdesign:** указатель на функцию дизайна иконки батареи  
**x0:** начальные координаты по x верхнего левого угла иконки батареи  
**y0:** начальные координаты по y верхнего левого угла иконки батареи  

#### void batt_Plot(t_batt batt)  
- **Описание функции**  
Функция построения иконки батареи 
- **Параметры**
**batt:** переменная типа t_batt    

#### void batt_ChrctrstcProscessing(t_batt *ptrbatt)
- **Описание функции**    
Функция производит измерение напряжения и расчета процента заряда батареи   
- **Параметры**
**ptrbatt:** указатель на переменную типа t_batt  

#### void batt_PrintPercent(t_batt batt, int16_t x0, int16_t y0, uint16_t color) 
- **Описание функции**  
Функция выводит процент заряда батареи  
- **Параметры**  
**batt:** переменная типа t_batt  
**x0:** начальные координаты по x начала текста  
**y0:** начальные координаты по y начала текста  
**color:** цвет текста

#### Инициализация
Создать переменную типа t_batt
```C
t_batt testBat;
```
Вызвать функцию инициализации.
```C
batt_Init(&testBat, greenbattery, 10, 10);
```
В цикле while вызвать функцию batt_ChrctrstcProscessing, для вычисления параметров батареи. По желанию вызывать функцию отображения % заряда батареи.
```C
while (1)
{	
  batt_ChrctrstcProscessing(&testBat);
  batt_Plot(testBat);
  
  batt_PrintPercent(testBat, 43, 13, ST7735_COLOR_WHITE);
}
```
#### Дизайн
Доступные варианты дизайна иконки батареи:
```C
batt_Init(&testBat, greenbattery, 10, 10);
```
<img width="200" height="250" alt="your alt text" src="https://user-images.githubusercontent.com/91386093/236017319-c35c4c3d-150c-45e7-9a33-7b4d1809597e.gif"/>

```C
batt_Init(&testBat, gradientbattery, 10, 10);
```
<img width="200" height="250" alt="your alt text" src="https://user-images.githubusercontent.com/91386093/236018731-06a2a19e-5d6d-453b-8487-0a07cb46b002.gif"/>

```C
batt_Init(&testBat, fsgrnyllwrdbattery, 10, 10);
```
<img width="200" height="250" alt="your alt text" src="https://user-images.githubusercontent.com/91386093/236018743-ecf8d1c4-8468-4cab-804b-0bc228ac2bb2.gif"/>

---
### Кнопка
