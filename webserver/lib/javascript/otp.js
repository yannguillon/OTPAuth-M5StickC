// import JsCrypto from 'jscrypto'
// import { Buffer } from 'buffer'
// import BPlistParser from './lib/browserified-pblist-parser'
// import { AESCBCDecrypt, RNCryptorDecrypt } from './crypto'
// const baseUrl = ''
//
// function getNSData (archive, index) {
//   return archive[0].$objects[index]['NS.data']
// }
//
// function getObjects (archive, index) {
//   return archive[0].$objects[index]['NS.objects']
// }
//
// function getUID (archive, index, objectName) {
//   return archive[0].$objects[index][objectName].UID
// }
//
// async function handleFileSelect (evt) {
// //  evt.preventDefault()
//   const file = document.getElementById('archive').files[0]
//   const password = document.getElementById('password').value
//   if (file) {
//     const reader = new window.FileReader()
//
//     reader.onload = async (e) => {
//       const data = AESCBCDecrypt(e.target.result, JsCrypto.SHA256.hash('Authenticator'))
//
//       let decryptedData = null
//       await BPlistParser.parseFile(Buffer.from(data), (_err, result) => {
//         decryptedData = RNCryptorDecrypt(getNSData(result, 5), password)
//         if (decryptedData === null) {
//           window.alert('wrong password')
//           return null
//         }
//         next(decryptedData)
//       })
//     }
//
//     reader.readAsArrayBuffer(file)
//   }
// }
//
// async function next (data) {
//   BPlistParser.parseFile(Buffer.from(data), async function (_err, res) {
//     const folderIds = getObjects(res, 5).map((objects) => {
//       return objects.UID
//     })
//     const accountsIndexes = folderIds.map((folderId) => {
//       return getUID(res, folderId, 'accounts')
//     })
//
//     let accounts = []
//
//     accountsIndexes.forEach((accountIndex) => {
//       accounts = accounts.concat(getObjects(res, accountIndex).map((obj) => { return obj.UID }))
//     })
//     const accIndexes = accounts.map((account) => {
//       const baseIndex = res[0].$objects[account]
//       return {
//         secret: baseIndex.secret.UID,
//         issuer: baseIndex.issuer.UID,
//         label: baseIndex.label.UID
//       }
//     })
//     const final = Object.keys(accIndexes).map((index) => {
//       return {
//         secret: Array.from(res[0].$objects[accIndexes[index].secret]),
//         hmac_length: res[0].$objects[accIndexes[index].secret].length,
//         issuer: res[0].$objects[accIndexes[index].issuer],
//         label: res[0].$objects[accIndexes[index].label]
//       }
//     })
//
//     let i = 1
//     await fetch(`${baseUrl}/deleteAll`, { method: 'POST' })
//
//     for (const f of Object.keys(final)) {
//       await fetch(`${baseUrl}/add`, {
//         method: 'POST',
//         headers: { 'Content-Type': 'application/json' },
//         body: JSON.stringify({
//           id: i,
//           data: final[f].secret,
//           label: final[f].issuer,
//           user: final[f].label,
//           hmac_length: final[f].hmac_length
//         })
//       })
//       i += 1
//     }
//   })
// }
// //
// // document.getElementById('submit').addEventListener('click', handleFileSelect)
// //
