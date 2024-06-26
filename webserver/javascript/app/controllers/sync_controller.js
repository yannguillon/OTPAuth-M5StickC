import { Controller } from '@hotwired/stimulus'
import BPlistParser from '../../lib/browserified-pblist-parser'
import { Buffer } from 'buffer'
import { AESCBCDecrypt, RNCryptorDecrypt } from '../helpers/crypto'
import { fetchWithOK } from '../helpers/requests'
import { createAlert, createNotice } from '../helpers/flash'
import JsCrypto from 'jscrypto'
import {
  getNSData,
  getObjects,
  getUID
} from '../helpers/parser'

export default class extends Controller {
  static targets = ['archive', 'password']
  static baseUrl = ''

  async upload (e) {
    e.preventDefault()

    const file = this.archiveTarget.files[0]
    const password = this.passwordTarget.value
    if (file) {
      const reader = new window.FileReader()
      reader.onload = async (e) => {
        const data = AESCBCDecrypt(e.target.result, JsCrypto.SHA256.hash('Authenticator'))
        let decryptedData = null
        BPlistParser.parseFile(Buffer.from(data), (_err, result) => {
          decryptedData = RNCryptorDecrypt(getNSData(result, 5), password)
          if (decryptedData === null) {
            createAlert('Wrong Password')
            return null
          }
          this.next(decryptedData)
        })
      }

      reader.readAsArrayBuffer(file)
    }
  }

  async next (data) {
    BPlistParser.parseFile(Buffer.from(data), async function (_err, res) {
      const folderIds = getObjects(res, 5).map((objects) => {
        return objects.UID
      })
      const accountsIndexes = folderIds.map((folderId) => {
        return getUID(res, folderId, 'accounts')
      })

      let accounts = []

      accountsIndexes.forEach((accountIndex) => {
        accounts = accounts.concat(getObjects(res, accountIndex).map((obj) => { return obj.UID }))
      })
      const accIndexes = accounts.map((account) => {
        const baseIndex = res[0].$objects[account]
        return {
          secret: baseIndex.secret.UID,
          issuer: baseIndex.issuer.UID,
          label: baseIndex.label.UID
        }
      })
      const final = Object.keys(accIndexes).map((index) => {
        return {
          secret: Array.from(res[0].$objects[accIndexes[index].secret]),
          hmac_length: res[0].$objects[accIndexes[index].secret].length,
          issuer: res[0].$objects[accIndexes[index].issuer],
          label: res[0].$objects[accIndexes[index].label]
        }
      })

      let i = 1
      try {
        await fetchWithOK('/otps', 'DELETE', '')
      } catch (_e) {
        createAlert('Network error with M5Stick')
        return
      }

      for (const f of Object.keys(final)) {
        try {
          await fetchWithOK('/otps', 'POST', {
            id: i,
            data: final[f].secret,
            label: final[f].issuer,
            user: final[f].label,
            hmac_length: final[f].hmac_length
          })
        } catch (_e) {
          createAlert('Network error with M5Stick')
          return
        }
        i += 1
      }
      createNotice('Sync successful')
    })
  }
}
