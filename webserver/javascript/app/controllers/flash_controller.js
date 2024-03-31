import Notification from '@stimulus-components/notification'

export default class extends Notification {
  static classes = {
    notice: ['text-green-800', 'dark:text-green-400', 'bg-green-50'],
    alert: ['text-red-800', 'dark:text-red-400', 'bg-red-50']
  }

  static targets = ['content', 'flash']
  async show (event = null) {
    if (!event?.detail?.content || !event?.detail?.type) return

    this.contentTarget.innerText = event.detail.content
    if (event.detail.type === 'alert') {
      this.flashTarget.classList.remove(...this.constructor.classes.notice)
      this.flashTarget.classList.add(...this.constructor.classes.alert)
    } else {
      this.flashTarget.classList.add(...this.constructor.classes.notice)
      this.flashTarget.classList.remove(...this.constructor.classes.alert)
    }

    this.enter()
  }

  async hide () {
    if (this.timeout) {
      clearTimeout(this.timeout)
    }

    await this.leave()
  }
}
