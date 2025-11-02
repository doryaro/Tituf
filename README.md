# Tituf Game Engine
---

# 🧱 Building Tituf

## 📦 Prerequisites
- **Windows 10 or 11**
- **Git**
- **Premake 5**
- **Visual Studio 2022** (recommended, with the *Desktop development with C++* workload)

---

## ⚙️ Setup

### 1. Clone the repository (with submodules)
```bash
git clone --recursive https://github.com/doryaro/Tituf.git
cd Tituf
```
### 2. Generate Visual Studio project files

### 🟩 Option 1 — If you have Visual Studio 2022 installed
Simply run:
```bash
GenerateProjects.bat
``` 

This automatically runs:

```bash
premake5 vs2022
and generates the Tituf.sln solution file.
```

### 🟦 Option 2 — If you don’t use Visual Studio 2022
You can manually run Premake for a different environment:

```bash
premake5 <IDE>
or any other supported action (for example premake5 vs2019).
```

3. Open the generated solution/project
For Visual Studio: open Tituf.sln

4. Build and run

Executable location:
```bash
bin\Debug-windows-x86_64\Tituf\Tituf.exe
```
