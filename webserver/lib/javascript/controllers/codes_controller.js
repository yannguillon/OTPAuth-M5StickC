import { Controller } from "@hotwired/stimulus"

export default class extends Controller {
  sync(e) {
    e.preventDefault()
    console.log('SYNC!')
  }
}
