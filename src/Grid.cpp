#include "Grid.hpp"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


void Init(width, height) {
    this.width = width;
    this.height = height;
    this.grid = new Array(width * height).fill(0);
  }

  // Allow us to clear the canvas
  clear() {
    this.grid = new Array(this.width * this.height).fill(0);
  }

  // Allow us to set a specific particle
  set(x, y, color) {
    this.grid[y * this.width + x] = color;
  }

  // Allow us to swap two particles (or space)
  swap(a, b) {
    const temp = this.grid[a];
    this.grid[a] = this.grid[b];
    this.grid[b] = temp;
  }

  // Check if a particle exists in a space
  isEmpty(index) {
    return this.grid[index] === 0;
  }
}