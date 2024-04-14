function convertToRGB565 (color) {
  // Parse the color string to get the RGB values
  const r = parseInt(color.substring(1, 3), 16)
  const g = parseInt(color.substring(3, 5), 16)
  const b = parseInt(color.substring(5, 7), 16)

  // Convert RGB888 to RGB565
  const r565 = (r >> 3) & 0x1F
  const g565 = (g >> 2) & 0x3F
  const b565 = (b >> 3) & 0x1F

  // Combine the RGB565 components into a single integer
  const rgb565 = (r565 << 11) | (g565 << 5) | b565
  return rgb565
}

function convertToRGB888 (color) {
  let r = (color >> 11) & 0x1F
  let g = (color >> 5) & 0x3F
  let b = color & 0x1F

  r = (r << 3) | (r >> 2)
  g = (g << 2) | (g >> 4)
  b = (b << 3) | (b >> 2)

  return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`
}

export {
  convertToRGB565,
  convertToRGB888
}
