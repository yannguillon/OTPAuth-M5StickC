// Define the base endpoint address
// LEAVE BLANK IN PRODUCTION
const baseUrl = ''

// --- TAB SELECTION LOGIC ---
const _ = el => [...document.querySelectorAll(el)]
_('[role=tab]')[0].setAttribute('aria-current', true)

_('[role=tab]').forEach(tab => {
  tab.addEventListener('click', (e) => {
    e.preventDefault()

    if (!e.target.hasAttribute('aria-current')) { e.target.setAttribute('aria-current', true) }

    _('[role=tab]').forEach(t => {
      if (t.hasAttribute('aria-current') && t !== e.target) { t.removeAttribute('aria-current') }
    })

    _('[role=tabpanel]').forEach(tp => {
      if (_('[role=tabpanel]').indexOf(tp) === _('[role=tab]').indexOf(e.target)) { tp.removeAttribute('hidden') } else { tp.setAttribute('hidden', true) }
    })
  })
})

// --- GMT TIMEZONE LOGIC ---
document.getElementById('timezone').addEventListener('change', function () {
  const timezone = document.getElementById('timezone').value
  const payload = {
    timezone
  }

  fetch(`${baseUrl}/timezone`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  })
    .then(response => {
      return response.text()
    })
    .then(data => {
      console.log(data) // Log server response
    })
    .catch(error => {
      console.error('There was a problem with the fetch operation:', error)
    })
})

// --- UNIX SYNC TIME LOGIC ---

document.getElementById('unixButton').addEventListener('click', function () {
  // Get current Unix timestamp
  const unixTime = Math.floor(Date.now() / 1000)
  const payload = {
    unix: unixTime
  }

  fetch(`${baseUrl}/unix`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  })
    .then(response => {
      return response.text()
    })
    .then(message => {
      window.alert(message) // Display server response
    })
    .then(data => {
      console.log(data) // Log server response
    })
    .catch(error => {
      console.error('There was a problem with the fetch operation:', error)
    })
})

// --- SCREEN SETTINGS LOGIC ---
// from RGB565 to RGB888
function convertToRGB888 (color) {
  let r = (color >> 11) & 0x1F
  let g = (color >> 5) & 0x3F
  let b = color & 0x1F

  r = (r << 3) | (r >> 2)
  g = (g << 2) | (g >> 4)
  b = (b << 3) | (b >> 2)

  return `#${r.toString(16).padStart(2, '0')}${g.toString(16).padStart(2, '0')}${b.toString(16).padStart(2, '0')}`
}

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

document.getElementById('setScreenSettings').addEventListener('click', function () {
  const screenTimeout = document.getElementById('screenTimeout').value
  const brightness = document.getElementById('brightness').value
  const txtColor = document.getElementById('txt_color').value
  const bgColor = document.getElementById('bg_color').value
  // Convert the colors to RGB565 format
  const txtColor565 = convertToRGB565(txtColor)
  const bgColor565 = convertToRGB565(bgColor)
  const payload = {
    timeout: screenTimeout,
    brightness,
    bg_color: bgColor565,
    txt_color: txtColor565
  }

  fetch(`${baseUrl}/setScreen`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  })
    .then(response => {
      return response.text()
    })
    .then(message => {
      window.alert(message) // Display server response
    })
    .then(data => {
      console.log(data) // Log server response
    })
    .catch(error => {
      console.error('There was a problem with the fetch operation:', error)
    })
})

// fetch initial values from server
fetch(`${baseUrl}/getScreen`)
  .then(response => response.json())
  .then(data => {
    document.getElementById('screenTimeout').value = data.timeout
    document.getElementById('brightness').value = data.brightness
    document.getElementById('bg_color').value = convertToRGB888(data.bg_color)
    document.getElementById('txt_color').value = convertToRGB888(data.txt_color)
  })
  .catch(error => console.error('Error fetching initial values:', error))

// get timezone from server
fetch(`${baseUrl}/timezone`)
  .then(response => response.json())
  .then(data => {
    document.getElementById('timezone').value = data.timezone
  })
  .catch(error => console.error('Error fetching initial values:', error))

// --- GET OTP LIST LOGIC ---
fetch(`${baseUrl}/getOTPs`)
  .then(response => response.json())
  .then(data => {
  // Process the received data and generate HTML
    const otpList = document.getElementById('otpList')
    data.OTPs.forEach(entry => {
      const listItem = document.createElement('li')
      listItem.classList.add('otpItem')
      listItem.innerHTML = `
          <strong>ID: </strong>${entry.id}<br>
          <strong>Label:</strong> ${entry.L}<br>
          <strong>User:</strong> ${entry.U}
          <button class="deleteButton" onclick="deleteOTP(${entry.id})">delete</button>
        `
      otpList.appendChild(listItem)
    })
  })
  .catch(error => console.error('Error fetching data:', error))

// ! --- CONFIGURATION LOGIC ---

// Function to populate fields with initial values
function populateFields (data) {
  document.getElementById('wifiModeSelect').value = data.mode
  document.getElementById('ssidInput').value = data.ssid
  // Password field is intentionally left blank for security reasons
  document.getElementById('passwordModeSelect').value = data.passwordMode
  togglePasswordModeVisibility() // Toggle password mode visibility based on wifi mode
}

// Fetch initial values from server
fetch(`${baseUrl}/getWifi`)
  .then(response => response.json())
  .then(data => {
    populateFields(data)
  })
  .catch(error => console.error('Error fetching initial values:', error))

// Toggle password mode visibility based on wifi mode
function togglePasswordModeVisibility () {
  const wifiMode = document.getElementById('wifiModeSelect').value
  const passwordModeDiv = document.getElementById('passwordModeDiv')
  const ssidInput = document.getElementById('ssidInput')
  passwordModeDiv.style.display = wifiMode === 'STA' ? 'none' : 'block'
  ssidInput.disabled = wifiMode !== 'STA'
}

// Apply button click event
document.getElementById('applyWifiButton').addEventListener('click', function () {
  const wifiMode = document.getElementById('wifiModeSelect').value
  const ssid = document.getElementById('ssidInput').value
  const password = document.getElementById('passwordInput').value
  const passwordMode = document.getElementById('passwordModeSelect').value

  const payload = {
    mode: wifiMode,
    ssid,
    password,
    passwordMode
  }

  fetch(`${baseUrl}/setWifi`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(payload)
  })
    .then(response => response.text())
    .then(message => {
      window.alert(message) // Display server response
    })
    .catch(error => {
      console.error('Error applying settings:', error)
    })
})

// Add event listener to wifi mode select to toggle password mode visibility
document.getElementById('wifiModeSelect').addEventListener('change', togglePasswordModeVisibility)
// ! --- end of configuration logic ---

// factory reset button

document.getElementById('factoryReset').addEventListener('click', function () {
  if (window.confirm('Are you sure you want to reset the device?')) {
    fetch(`${baseUrl}/factoryReset`, {
      method: 'POST'
    })
      .then(response => response.text())
      .then(message => {
        window.alert(message) // Display server response
      })
      .catch(error => {
        console.error('Error resetting device:', error)
      })
  }
})

// ! --- FIRMWARE UPDATE LOGIC ---

// Enable or disable the update button based on whether a file is selected
document.getElementById('file').addEventListener('change', function () {
  document.getElementById('update-button').style.display = this.value ? 'block' : 'none'
})

document.getElementById('upload_form').addEventListener('submit', function (e) {
  e.preventDefault()
  const form = document.getElementById('upload_form')
  const data = new window.FormData(form)
  const xhr = new window.XMLHttpRequest()

  xhr.upload.addEventListener('progress', function (evt) {
    if (evt.lengthComputable) {
      const per = evt.loaded / evt.total
      document.getElementById('prg').innerHTML = 'progress: ' + Math.round(per * 100) + '%'
      document.getElementById('bar').style.width = Math.round(per * 100) + '%'
    }
  }, false)

  xhr.open('POST', `${baseUrl}/update`, true)
  xhr.send(data)
  xhr.onload = function () {
    console.log('success!')
    window.alert('Firmware updated successfully!')
  }
  xhr.onerror = function () {
    console.error('error!')
    window.alert('Error: Unable to reach the server.')
  }
})

// ! --- end of firmware update logic ---
