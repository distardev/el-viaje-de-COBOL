# 🎮 El viaje de COBOL

**Versión:** Beta 0.2.0  
**Motor:** C++ + [SFML](https://www.sfml-dev.org/)  
**Desarrollador principal:** distardev

> Acompaña a COBOL en una aventura épica por las tierras de la computación clásica, enfrentando esqueletos errantes y criaturas flameantes, recolectando monedas y mejorando su arsenal. ¿Lograrás llegar al final?

---

![Game_Screenshot0](https://github.com/user-attachments/assets/e84a0ae0-1273-4992-ade3-df7394289a5a)
![Game_Screenshot1](https://github.com/user-attachments/assets/7924b75a-69a5-4221-9007-58e498de19ff)
![Game_Screenshot2](https://github.com/user-attachments/assets/f305e241-e381-45ca-ae54-0077269490bb)


## 🕹️ Características principales

- 👾 Enemigos variados:
  - **Skeletons:** persiguen al jugador.
  - **Fired Skeletons:**  estos son más furiosos y corren más rápido.
  - **Pitcher Skeletons:** lanzan bolas de fuego letales.
- 🎯 Mira dinámica controlable con mouse o joystick.
- 💥 Sistema de disparo con power-ups y niveles de fuego.
- ❤️ Sistema de vida.
- 💰 Recolección de monedas.
- 🎵 Música y efectos de sonido personalizados.
- ⏸️ Menú de pausa con control de volumen (música, efectos).
- 🧠 IA simple que persigue al jugador y ataca.
- 🌌 Fondo dinámico animado con shader.

---

## 🛠️ Tecnologías usadas

- **Lenguaje:** C++
- **Motor gráfico:** [SFML 2.6](https://www.sfml-dev.org/)
- **IDE recomendada:** Visual Studio Code
- **Audio y música:**  
  - [Pixabay Music](https://pixabay.com/music/)
- **Gráficos pixel art:** Spritesheets diseñados y animados manualmente.

---

## 📦 Estructura del proyecto
```bash

el_viaje_de_cobol/
├── assets/
│   ├── sprites/
│   ├── audio/
│   └── shaders/
├── src/
│   ├── main.cpp
│   ├── player.cpp/.hpp
│   ├── enemy.cpp/.hpp
│   ├── ui.cpp/.hpp
│   ├── money.cpp/.hpp
│   ├── audio_manager.cpp/.hpp
│   ├── menu.cpp/.hpp
│   └── dynamic_background.cpp/.hpp
├── README.md
└── Makefile (o build.sh)
```
---

## 🔧 Instalación y ejecución

### Linux

```bash
# Requiere SFML
sudo apt install libsfml-dev

# Clona el repositorio
git clone https://github.com/tu-usuario/el-viaje-de-cobol.git
cd el-viaje-de-cobol

# Compila
g++ src/*.cpp -o cobol -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Ejecuta
./cobol
```

## 🧠 Inspiración

Inspirado en la estética del minijuego de la taberna "el viaje del rey de la pradera" del videojuego Stardew Valley.


![JOPK_Level_2_1](https://github.com/user-attachments/assets/d5c4e161-94f8-4da7-8294-5e99874391d5)
![JOPK_Level_1_2](https://github.com/user-attachments/assets/43e0a8e4-12f7-4dc4-8600-28cba934c09f)
![JOPK_logo_ES](https://github.com/user-attachments/assets/a26df50e-af87-4d2b-896e-206b72908931)

## 🐛 Reportar errores

Si encontrás bugs o problemas, por favor abrí un [Issue](https://github.com/distardev/el-viaje-de-COBOL/issues/) con una descripción clara y, si es posible, capturas del error.
