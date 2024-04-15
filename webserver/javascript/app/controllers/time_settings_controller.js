import { Controller } from '@hotwired/stimulus'
import { fetchWithOK } from '../helpers/requests'
import { createAlert, createNotice } from '../helpers/flash'

export default class extends Controller {
  static targets = ['timezone']

  async connect () {
    try {
      const response = await fetchWithOK('/config/timezone', 'GET')
      const parsedResponse = await response.json()

      this.timezoneTarget.value = parsedResponse.timezone
    } catch (_e) {
      createAlert('Network error with M5Stick (fetching timezone configuration)')
    }
  }

  async updateTimezone (e) {
    e.preventDefault()
    try {
      await fetchWithOK('/config/timezone', 'PATCH', {
        timezone: this.timezoneTarget.value
      })
    } catch (_e) {
      createAlert('Network error with M5Stick')
      return
    }
    createNotice('Timezone Updated')
  }

  async syncTime (e) {
    e.preventDefault()
    try {
      await fetchWithOK('/config/time', 'PATCH', {
        unix: Math.floor(Date.now() / 1000)
      })
    } catch (_e) {
      createAlert('Network error with M5Stick')
      return
    }
    createNotice('Sync successful')
  }
}
