import { Controller } from '@hotwired/stimulus'
import { fetchWithOK } from '../helpers/requests'
import { createAlert, createNotice } from '../helpers/flash'
import { convertToRGB565, convertToRGB888 } from '../helpers/colors'

export default class extends Controller {
  static targets = ['timeout', 'brightness', 'textColor', 'backgroundColor']

  async connect () {
    try {
      const response = await fetchWithOK('/config/display', 'GET')
      const parsedResponse = await response.json()

      this.brightnessTarget.value = parsedResponse.brightness
      this.timeoutTarget.value = parsedResponse.timeout
      this.backgroundColorTarget.value = convertToRGB888(parsedResponse.bg_color)
      this.textColorTarget.value = convertToRGB888(parsedResponse.txt_color)
    } catch (_e) {
      createAlert('Network error with M5Stick (fetching screen configuration)')
    }
  }

  async update (e) {
    e.preventDefault()
    const body = {
      brightness: this.brightnessTarget.value,
      timeout: this.timeoutTarget.value,
      bg_color: convertToRGB565(this.backgroundColorTarget.value),
      txt_color: convertToRGB565(this.textColorTarget.value)
    }

    try {
      await fetchWithOK('/config/display', 'PATCH', body)
    } catch (_e) {
      createAlert('Network error with M5Stick')
      return
    }
    createNotice('Screen settings updated')
  }
}
