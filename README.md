# Polynomial & Calculus Toolkit

A high-performance C++ engine and interactive Web UI for polynomial algebra, numerical calculus operations, and function analysis.

## Features
- **Manual Dynamic Memory Management**: Built from scratch using raw pointers without relying on standard library vectors.
- **Operator Overloading**: Native support for addition (`+`), subtraction (`-`), multiplication (`*`), copy assignment (`=`), and stream input/output (`>>`, `<<`).
- **Calculus Suite**: Nth-order derivatives, indefinite/definite integration, Newton-Raphson root estimation, and critical point classification (local minima, maxima, inflection).
- **Geometric & Algebraic Analysis**: Custom tangent/normal line calculation, polynomial synthetic division with remainder, and polynomial composition.
- **Interactive CLI & Web UI**: Feature-rich menu-driven terminal interface alongside a Chart.js web application.

---

## C++ CLI Setup & Usage

### Building the Project
```bash
mkdir build
cd build
cmake ..
make
./PolynomialToolkit