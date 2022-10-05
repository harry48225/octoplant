# octoplant
Plant water indicator

octoplant helps you keep your plant hydrated. Using 12 LEDs it shows the current soil moisture level and allows you to set the water point so you can easily see how far off you are from the moisture level at which you normally water.

It's designed around an attiny824. A custom PCB combines it along with 12 LEDs, a wide trace that acts as a capacitive moisture sensor, a button, and a cr2032 battery (holder).

Simply press the button to light the leds. The soil moisture level is measured and then compared to the calibrated wet and dry points. By holding down the button you can save a moisture level and this led will blink on all further readings (I like to use this to save the level of moisture I usually water at). 

Built with:
 - Arduino (TinyMega core)
 - kicad
 - platform io

![octoplant in an aparagus fern](/images/light_in_plant.jpg)

![octoplant front wide](/images/front_wide.jpg)

![octoplant back wide](/images/back.jpg)

![octoplant front wide off](/images/off_in_plant.jpg)

![octoplant front close](/images/front_close.jpg)