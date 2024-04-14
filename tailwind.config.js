/** @type {import('tailwindcss').Config} */
module.exports = {
  content: [
    './webserver/public/index.html'
  ],
  plugins: [
    require('flowbite/plugin')
  ],
  safelist: [
    'text-green-800',
    'dark:text-green-400',
    'bg-green-50',
    'text-red-800',
    'dark:text-red-400',
    'bg-red-50',
    'fill-blue-600'
  ],
  darkMode: 'class'
}
