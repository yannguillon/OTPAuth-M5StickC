import { Controller } from '@hotwired/stimulus'
import { fetchWithOK } from '../helpers/requests'
import { createAlert, createNotice } from '../helpers/flash'
import { convertToRGB888 } from '../helpers/colors'

export default class extends Controller {
  static targets = ['mode', 'ssid', 'password', 'passwordMode']

  async connect () {
    try {
      const response = await fetchWithOK('/config/wlan', 'GET')
      const parsedResponse = await response.json()

      this.modeTarget.value = parsedResponse.mode
      this.ssidTarget.value = parsedResponse.ssid
      this.passwordModeTarget.value = parsedResponse.passwordMode
    } catch (_e) {
      createAlert('Network error with M5Stick (fetching wlan configuration)')
    }
  }

  async update (e) {
    e.preventDefault()
    try {
      await fetchWithOK('/config/wlan', 'PATCH', {
        mode: this.modeTarget.value,
        ssid: this.ssidTarget.value,
        password: this.passwordTarget.value,
        passwordMode: this.passwordModeTarget.value
      })
    } catch (_e) {
      createAlert('Network error with M5Stick')
      return
    }
    createNotice('Wlan Settings Updated')
  }
}
