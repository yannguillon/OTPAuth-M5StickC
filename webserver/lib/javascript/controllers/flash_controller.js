import { Controller } from "@hotwired/stimulus"

export default class extends Controller {
  static storageKey = 'dev-mode'
  static targets = ['devModeButton', 'devModeIcon']

  connect() {
    this.storageKey = this.constructor.storageKey
    const storageValue = window.localStorage.getItem(this.storageKey)
    if (storageValue === null) window.localStorage.setItem(this.storageKey, false.toString())

    const devModeEnabled = window.localStorage.getItem(this.storageKey) === 'true'
    this.update(devModeEnabled)
  }

  update(devModeEnabled) {
    if (devModeEnabled) {
      this.devModeButtonTarget.classList.add('border-blue-600')
      this.devModeIconTarget.classList.remove('fill-gray-400')
      this.devModeIconTarget.classList.add('fill-blue-600')

      document.querySelectorAll('[class*="shown-dev-mode"]').forEach((selector) => {
        selector.classList.remove('hidden')
      })
      document.querySelectorAll('[class*="hidden-dev-mode"]').forEach((selector) => {
        selector.classList.add('hidden')
      })
    }
    else {
      this.devModeButtonTarget.classList.remove('border-blue-600')
      this.devModeIconTarget.classList.remove('fill-blue-600')
      this.devModeIconTarget.classList.add('fill-gray-400')

      document.querySelectorAll('[class*="hidden-dev-mode"]').forEach((selector) => {
        selector.classList.remove('hidden')
      })

      document.querySelectorAll('[class*="shown-dev-mode"]').forEach((selector) => {
        selector.classList.add('hidden')
      })
    }
  }

  toggle() {
    const devModeEnabled = !(window.localStorage.getItem(this.storageKey) === 'true')
    window.localStorage.setItem(this.storageKey, devModeEnabled.toString())
    this.update(devModeEnabled)
  }
}
