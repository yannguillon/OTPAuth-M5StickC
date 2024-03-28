import JsCrypto from 'jscrypto'
import { Buffer } from 'buffer'
import BPlistParser from './lib/browserified-pblist-parser'
import { AESCBCDecrypt, RNCryptorDecrypt } from './crypto'
const baseUrl = ''

function getNSData (archive, index) {
  return archive[0].$objects[index]['NS.data']
}

function getObjects (archive, index) {
  return archive[0].$objects[index]['NS.objects']
}

function getUID (archive, index, objectName) {
  return archive[0].$objects[index][objectName].UID
}

async function handleFileSelect (evt) {
  evt.preventDefault()
  const file = document.getElementById('archive').files[0]
  const password = document.getElementById('password').value
  console.log(file)
  console.log(password)
  if (file) {
    const reader = new window.FileReader()

    reader.onload = async (e) => {
      const data = AESCBCDecrypt(e.target.result, JsCrypto.SHA256.hash('Authenticator'))

      let decryptedData = null
      await BPlistParser.parseFile(Buffer.from(data), (_err, result) => {
        decryptedData = RNCryptorDecrypt(getNSData(result, 5), password)
        if (decryptedData === null) {
          window.alert('wrong password')
          return null
        }
        next(decryptedData)
      })
    }

    reader.readAsArrayBuffer(file)
  }
}

async function next (data) {
  BPlistParser.parseFile(Buffer.from(data), async function (_err, res) {
    const folderIds = getObjects(res, 5).map((objects) => {
      return objects.UID
    })
    const accountsIndexes = folderIds.map((folderId) => {
      return getUID(res, folderId, 'accounts')
    })

    let accounts = []

    accountsIndexes.forEach((accountIndex) => {
      accounts = accounts.concat(getObjects(res, accountIndex).map((obj) => { return obj.UID }))
    })
    const accIndexes = accounts.map((account) => {
      const baseIndex = res[0].$objects[account]
      return {
        secret: baseIndex.secret.UID,
        issuer: baseIndex.issuer.UID,
        label: baseIndex.label.UID
      }
    })
    const final = Object.keys(accIndexes).map((index) => {
      return {
        secret: Array.from(res[0].$objects[accIndexes[index].secret]),
        hmac_length: res[0].$objects[accIndexes[index].secret].length,
        issuer: res[0].$objects[accIndexes[index].issuer],
        label: res[0].$objects[accIndexes[index].label]
      }
    })

    let i = 1
    await fetch(`${baseUrl}/deleteAll`, { method: 'POST' })

    for (const f of Object.keys(final)) {
      await fetch(`${baseUrl}/add`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          id: i,
          data: final[f].secret,
          label: final[f].issuer,
          user: final[f].label,
          hmac_length: final[f].hmac_length
        })
      })
      i += 1
    }
  })
}

document.getElementById('submit').addEventListener('click', handleFileSelect)

// On page load or when changing themes, best to add inline in `head` to avoid FOUC
if (window.localStorage.getItem('color-theme') === 'dark' || (!('color-theme' in window.localStorage) && window.matchMedia('(prefers-color-scheme: dark)').matches)) {
  document.documentElement.classList.add('dark')
} else {
  document.documentElement.classList.remove('dark')
}

const themeToggleDarkIcon = document.getElementById('theme-toggle-dark-icon')
const themeToggleLightIcon = document.getElementById('theme-toggle-light-icon')
const devModeToggle = document.getElementById('dev-mode-toggle')
const devModeIcon = document.getElementById('dev-mode-icon')

// Change the icons inside the button based on previous settings
if (window.localStorage.getItem('color-theme') === 'dark' || (!('color-theme' in window.localStorage) && window.matchMedia('(prefers-color-scheme: dark)').matches)) {
  themeToggleLightIcon.classList.remove('hidden')
} else {
  themeToggleDarkIcon.classList.remove('hidden')
}

const themeToggleBtn = document.getElementById('theme-toggle')

themeToggleBtn.addEventListener('click', function () {
  // toggle icons inside button
  themeToggleDarkIcon.classList.toggle('hidden')
  themeToggleLightIcon.classList.toggle('hidden')

  // if set via local storage previously
  if (window.localStorage.getItem('color-theme')) {
    if (window.localStorage.getItem('color-theme') === 'light') {
      document.documentElement.classList.add('dark')
      window.localStorage.setItem('color-theme', 'dark')
    } else {
      document.documentElement.classList.remove('dark')
      window.localStorage.setItem('color-theme', 'light')
    }

    // if NOT set via local storage previously
  } else {
    if (document.documentElement.classList.contains('dark')) {
      document.documentElement.classList.remove('dark')
      window.localStorage.setItem('color-theme', 'light')
    } else {
      document.documentElement.classList.add('dark')
      window.localStorage.setItem('color-theme', 'dark')
    }
  }
})

function updateDevMode (e, toggle = true) {
  if (toggle === true) {
    window.localStorage.setItem('dev-mode', (!(window.localStorage.getItem('dev-mode') === 'true')).toString())
  }
  const devMode = window.localStorage.getItem('dev-mode') === 'true'
  if (devMode) {
    devModeToggle.classList.add('border-blue-600')
    devModeIcon.classList.remove('fill-gray-400')
    devModeIcon.classList.add('fill-blue-600')
  } else {
    devModeToggle.classList.remove('border-blue-600')
    devModeIcon.classList.remove('fill-blue-600')
    devModeIcon.classList.add('fill-gray-400')
  }
}

updateDevMode(null, false)
devModeToggle.addEventListener('click', updateDevMode)
