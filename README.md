# 🔦 miniRT

**miniRT** es mi primer motor gráfico construido desde cero en **C**</br>El objetivo: generar imágenes 3D realistas simulando la física de la luz mediante la técnica de **Raytracing**

<p align="center">
  <img width="1697" height="850" alt="minirtscenes" src="https://github.com/user-attachments/assets/99eb3101-560e-4634-b5b5-8ec5755ad7e3" />
</p>

## ¿Cómo funciona la magia? 🧠

En el mundo real, la luz sale de una fuente (el Sol, una bombilla), rebota en los objetos y llega a tus ojos. Pero simular los infinitos rayos de luz del universo es imposible para un ordenador.

**Así que lo hacemos al revés.**

En lugar de seguir la luz, seguimos la **visión**:
1.  Lanzamos un "rayo" desde la cámara (tu ojo) por cada píxel de la pantalla.
2.  Calculamos matemáticamente si ese rayo choca contra algún objeto (esferas, planos o cilindros).
3.  Si choca, lanzamos un segundo rayo desde ese punto hacia la luz para ver si algo obstruye el camino (sombras).
4.  Aplicamos fórmulas físicas para determinar el color exacto basándonos en el ángulo de la luz y el material.

## 🛠️ Características

Este proyecto utiliza la librería **MiniLibX** y soporta:

- **Primitivas Geométricas:** Renderizado de Planos, Esferas y Cilindros.
- **Sistema de Iluminación:** Implementación del modelo de reflexión de Phong (Luz Ambiental, Difusa y Especular).
- **Sombras:** Cálculo de "Hard shadows" para dar profundidad a la escena.
- **Cámara Móvil:** Control de posición, orientación y campo de visión (FOV).
- **Transformaciones:** Traslación y rotación de objetos y cámara.

## 📚 Lo que he aprendido

Este proyecto ha sido el desafío matemático más grande hasta la fecha en el cursus:

- **Matemáticas Vectoriales:** He tenido que traducir fórmulas matemáticas a código C puro. Todo son vectores: la dirección de la cámara, la normal de una superficie, la incidencia de la luz...
- **Álgebra Lineal:** Implementación de matrices de rotación (Fórmula de Rodrigues) para orientar la cámara y los objetos en el espacio 3D.
- **Resolución de Ecuaciones Cuadráticas:** Para calcular la intersección precisa de un rayo con un cilindro o una esfera, hay que resolver ecuaciones de segundo grado en tiempo real.
- **Pipeline de Renderizado:** Entender cómo convertir coordenadas 3D de un "mundo virtual" a píxeles 2D en una pantalla.

### Ejemplo de Escena (.rt)

El programa lee archivos de configuración simples para montar la escena:

```text
A 0.2 255,255,255          # Luz Ambiente (Intensidad, Color)
C -50,0,20 0,0,1 70        # Cámara (Posición, Orientación, FOV)
L -40,0,30 0.7 255,255,255 # Luz Puntual (Posición, Intensidad, Color)
sp 0,0,20 20 255,0,0       # Esfera Roja (Posición, Radio, Color)
cy 50,0,20.6 0,0,1 14.2 21.42 10,0,255 # Cilindro Azul (Posición, Orientación, Radio, Altura, Color)
