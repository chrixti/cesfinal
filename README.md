# Rock, Paper, Scissors ESP32 Game

This project is a 1v1 **Rock, Paper, Scissors game** built using ESP32 boards. Players use buttons on their ESP32 devices to select and confirm their moves, and the results are displayed on the onboard screens. The boards communicate wirelessly using ESP-NOW to ensure a seamless and real-time gaming experience.

### **Blog Post Link**
https://gusty-tail-7d3.notion.site/1v1-Rock-Paper-Scissors-14a5621b054b809da6b1c0c57c539f3f?pvs=4
---

### **Design Goals**

- Create a portable, wireless **Rock, Paper, Scissors** game using ESP32 boards.
- Ensure privacy for each player's moves by displaying choices on the ESP32 screens.
- Incorporate retro pixel art using bitmaps for a nostalgic gaming feel.
- Provide auditory feedback for move confirmation and game results using small speakers.
- Keep the hardware setup simple and easy to replicate.

---

### **Gameplay Workflow**

1. **Player Setup**: Each player uses an ESP32 board.
2. **Game Interaction**:
   - **Left Button**: Cycles through Rock, Paper, Scissors options.
   - **Right Button**: Confirms the selected move.
3. **Wireless Communication**: ESP-NOW handles real-time communication between the boards.
4. **Game Results**: After both players confirm their moves, results are displayed on each ESP32 screen.

---

### **Installation Instructions**

#### **Hardware Setup**
- **Components**:
  - 2 ESP32 boards (one per player).
  - 1 speaker per ESP32 for sound feedback.
  - Power supply via USB.
  
- **Circuit Diagram**:
![Fritzing Diagram]
<img width="430" alt="esp32-speaker" src="https://github.com/user-attachments/assets/3fd04f88-de25-4dcc-bb6b-5c91b9ad6140" />


#### **3D Enclosure**
- Print the provided 3D enclosure files to house the ESP32 boards.
- Ensure slots for buttons, USB ports, and the speaker are aligned properly.

---

### **Code and Files**

#### **File Structure**
player 1 folder/
├── paper_bitmap.h
├── scissors_bitmap.h
├── rock_bitmap.h
├── player1.ino

player 2 folder/
├── paper_bitmap.h
├── scissors_bitmap.h
├── rock_bitmap.h
├── player2.ino

1. **player1.ino**: Code for Player 1’s ESP32.
2. **player2.ino**: Code for Player 2’s ESP32.
3. **Bitmap Files**: These header files contain the pixel art for Rock, Paper, and Scissors.

#### **Setup Steps**
1. Clone the repository to your local machine:
   ```bash
   git clone https://github.com/chrixti/cesfinal.git

---

### **Video of the Game in Action**

### **Supplementary Images**

### **Acknowledgments**




