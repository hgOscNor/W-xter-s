import { initializeApp } from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-app.js'
import { getDatabase, ref } from 'https://www.gstatic.com/firebasejs/11.1.0/firebase-database.js'

const firebaseConfig = {
  apiKey: 'AIzaSyC3sxf274LVPT_RTw-I5TS4N6c3YbaMql4',
  authDomain: 'wexteras-8d3c3.firebaseapp.com',
  databaseURL: 'https://wexteras-8d3c3-default-rtdb.europe-west1.firebasedatabase.app',
  projectId: 'wexteras-8d3c3',
  storageBucket: 'wexteras-8d3c3.firebasestorage.app',
  messagingSenderId: '896283578937',
  appId: '1:896283578937:web:cc315228718995ae7273e8',
}

// Initialisera Firebase
const app = initializeApp(firebaseConfig)

// Hämta databasen
const db = getDatabase(app)

// Skapa en referens
const dataRef = ref(db, 'control')

export { app, db, dataRef }
