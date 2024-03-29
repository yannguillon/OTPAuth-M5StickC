import { Controller } from "@hotwired/stimulus"

export default class extends Controller {
  static classes = ['active']
  static targets = ["btn", "tab"]
  static values = {defaultTab: String}

  connect() {
    this.btnTargets[0].classList.add(...this.activeClasses)
    // let selectedTab = this.tabTargets.find(element => element.id === this.defaultTabValue)
    // selectedTab.hidden = false
    // let selectedBtn = this.btnTargets.find(element => element.id === this.defaultTabValue)
    // selectedBtn.classList.add(...this.activeClasses)
  }

  select(event) {
    let selectedTab = this.tabTargets.find(element => element.id === event.params.active)
    if (selectedTab.hidden) {
      this.tabTargets.map(x => x.hidden = true)
      this.btnTargets.map(x => x.classList.remove(...this.activeClasses))
      selectedTab.hidden = false
      event.currentTarget.classList.add(...this.activeClasses)
    }
  }
}
