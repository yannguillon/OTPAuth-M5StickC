/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    './webserver/public/index.html'
  ],
  plugins: [
    require('flowbite/plugin')
  ],
  darkMode: 'class',
  // ...
}
