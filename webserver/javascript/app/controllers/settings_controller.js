import { Controller } from '@hotwired/stimulus'
import { fetchWithOK } from "../helpers/requests"
import { createAlert, createNotice } from "../helpers/flash"

export default class extends Controller {
  static targets = ['timezone']

  async updateTimezone () {
    try {
      await fetchWithOK('/timezone', 'POST', {
        timezone: this.timezoneTarget.value
      })
    } catch (_e) {
    createAlert('Network error with M5Stick')
      return
    }
    createNotice('Timezone Updated' )
  }

  async syncTime () {
    try {
      const response = await fetchWithOK('/unix', 'POST', {
        unix: Math.floor(Date.now() / 1000)
      })
    } catch (_e) {
      createAlert('Network error with M5Stick')
      return
    }
    createNotice('Sync successful' )
  }
}
