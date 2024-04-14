import { Controller } from '@hotwired/stimulus'
import { fetchWithOK } from '../helpers/requests'
import { createAlert, createNotice } from '../helpers/flash'

export default class extends Controller {
  async reset (e) {
    e.preventDefault()
    try {
      await fetchWithOK('/reset', 'POST', {})
    } catch (_e) {
      createAlert('Network error with M5Stick')
      return
    }
    createNotice('M5 Stick initialized, Please reconnect')
  }
}
