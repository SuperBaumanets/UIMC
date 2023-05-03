## Дизайн пользовательского интерфейса
(Для отрисовки графических примитивов используется tft экран со схемой ST7735S)
Содержит следующие элементы пользовательского интерфейса: 
* Заряд аккамулятора
* Кнопка

---
### Заряд аккамулятора
#### Инициализация
Создать переменную типа t_batt
```C
t_batt testBat;
```
Вызвать функцию инициализации. Функция принимает в качестве аргументов адрес типа t_batt, дизайн иконки батареи и начальные координаты от которых будет строиться иконка(верхний левый угол).
```C
batt_Init(&testBat, greenbattery, 10, 10);
```
В цикле while вызвать функцию batt_ChrctrstcProscessing, для измерения напряжения и расчета процента заряда батареи, функция в качестве аргумента принимает адрес типа t_batt. Для построения иконки батареи необходимо вызвать функцию batt_Plot, функция в качестве аргумента принимает переменную типа t_batt. 
```C
while (1)
{	
  batt_ChrctrstcProscessing(&testBat);
  batt_Plot(testBat);
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

#### Дополнительные функции
* Функция batt_PrintPercent           
Выводит процент заряда батареи. Должна вызываться в цикле while, а в качестве аргументов принимает переменную типа t_batt, начальные координаты от которых будет производится печать и текст шрифта.  
```C
while (1)
{	
 batt_PrintPercent(testBat, 43, 13, ST7735_COLOR_WHITE);
}
```
---
### Кнопка
