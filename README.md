## 📝 TexEdit++

A minimal C++ text editor built with **wxWidgets**, designed to open and edit a text file provided via the command line.

---

### 🔧 Features

* Simple GUI with a resizable multiline textbox
* Loads a file at launch (via command line)
* Saves content on close (optional to add)
* Basic menu bar with `Hello` and `Exit` items

---

### 📷 Screenshot

> *(Optional: Add a screenshot here of the editor window once available)*

---

### 🚀 Getting Started

#### ✅ Prerequisites

* C++ compiler (e.g. `g++`)
* wxWidgets (3.x or newer)

#### ✅ Install wxWidgets (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install libwxgtk3.0-dev
```

#### ✅ Build the Project

```bash
g++ main.cpp `wx-config --cxxflags --libs` -o texedit
```

#### ✅ Run the Editor

```bash
./texedit myfile.txt
```

If `myfile.txt` exists, it will be loaded. If not, an empty editor opens (or the program exits, depending on the code logic).

---

### 📁 Project Structure

```
main.cpp     # Contains full source code
README.md    # This file
```

---

### ✨ Future Ideas

* Add file saving on exit
* Add toolbar with Open/Save buttons
* Syntax highlighting support
* Dark mode and font size settings

---

### 🧑‍💻 Author

* Built with ❤️ using wxWidgets

---
