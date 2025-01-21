import { defineBoot } from '#q-app/wrappers'
import { initializeApp } from 'firebase/app'
import { getDatabase } from 'firebase/database'
import { VueFire } from 'vuefire'

const firebaseApp = initializeApp({
  apiKey: 'AIzaSyC3sxf274LVPT_RTw-I5TS4N6c3YbaMql4',
  authDomain: 'wexteras-8d3c3.firebaseapp.com',
  databaseURL: 'https://wexteras-8d3c3-default-rtdb.europe-west1.firebasedatabase.app',
  projectId: 'wexteras-8d3c3',
  storageBucket: 'wexteras-8d3c3.firebasestorage.app',
  messagingSenderId: '896283578937',
  appId: '1:896283578937:web:cc315228718995ae7273e8',
})

const db = getDatabase(firebaseApp)

export default defineBoot(async ({ app }) => {
  app.use(VueFire, {
    firebaseApp,
  })
})

export { db }
