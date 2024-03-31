import { Controller } from '@hotwired/stimulus'

export default class extends Controller {
  static storageKey = 'color-theme'
  static targets = ['lightIcon', 'darkIcon']

  connect () {
    this.storageKey = this.constructor.currentThemeStorageKey
    let currentTheme = window.localStorage.getItem(this.storageKey)
    if (currentTheme === null) {
      currentTheme = 'dark'
      window.localStorage.setItem(this.storageKey, 'dark')
    }

    this.updateTheme(currentTheme)
  }

  updateTheme (currentTheme) {
    if (currentTheme === 'dark') {
      document.documentElement.classList.remove('light')
      document.documentElement.classList.add('dark')
      this.lightIconTarget.classList.remove('hidden')
      this.darkIconTarget.classList.add('hidden')
    } else {
      document.documentElement.classList.remove('dark')
      document.documentElement.classList.add('light')
      this.lightIconTarget.classList.add('hidden')
      this.darkIconTarget.classList.remove('hidden')
    }
  }

  toggle () {
    const newTheme = window.localStorage.getItem(this.storageKey) === 'dark' ? 'light' : 'dark'
    window.localStorage.setItem(this.storageKey, newTheme)
    this.updateTheme(newTheme)
  }
}
