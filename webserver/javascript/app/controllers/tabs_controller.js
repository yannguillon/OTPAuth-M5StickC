import { Controller } from '@hotwired/stimulus'

export default class extends Controller {
  static classes = ['active']
  static targets = ['btn', 'tab']
  static values = { defaultTab: String }

  connect () {
    this.btnTargets[0].classList.add(...this.activeClasses)
  }

  select (event) {
    const selectedTab = this.tabTargets.find(element => element.id === event.params.active)
    if (selectedTab.hidden) {
      this.tabTargets.forEach(x => { x.hidden = true })
      this.btnTargets.forEach(x => { x.classList.remove(...this.activeClasses) })
      selectedTab.hidden = false
      event.currentTarget.classList.add(...this.activeClasses)
    }
  }
}
