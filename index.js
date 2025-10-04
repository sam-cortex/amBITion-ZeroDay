import { initializeApp } from "https://www.gstatic.com/firebasejs/11.6.0/firebase-app.js";
import { getAuth } from "https://www.gstatic.com/firebasejs/11.6.0/firebase-auth.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/11.6.0/firebase-database.js";

// Firebase configuration as provided
const firebaseConfig = {
  apiKey: "AIzaSyDpqjicaEHzrPYMbO9VdgM9AI-ksqrLqI0",
  authDomain: "sensor-reading-6a414.firebaseapp.com",
  projectId: "sensor-reading-6a414",
  storageBucket: "sensor-reading-6a414.firebasestorage.app",
  messagingSenderId: "289032500561",
  appId: "1:289032500561:web:c2015d4cb92e2407a00f70"
};

// Initializeing Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const database = getDatabase(app);

// Export auth for use in auth.js
export { auth };

// UI Elements
const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");
const tempElement = document.getElementById("dist");

// Manage Login or Logout UI
const setupUI = (user) => {
  if (user) {
    // Toggle UI elements
    loginElement.style.display = 'none';
    contentElement.style.display = 'block';
    authBarElement.style.display = 'block';
    userDetailsElement.style.display = 'block';
    userDetailsElement.innerHTML = user.email;

    // Define database paths
    const uid = user.uid;
    const dbPathdist = 'UsersData/${uid}/dist';
    

    // Database references
    const dbRefdist = ref(database, dbPathdist);

    // Update page with new readings
    onValue(dbRefdist, (snap) => {
      distElement.innerText = snap.val()?.toFixed(2) ?? "N/A";
    });

  } else {
    // Toggle UI elements
    loginElement.style.display = 'block';
    authBarElement.style.display = 'none';
    userDetailsElement.style.display = 'none';
    contentElement.style.display = 'none';
  }
};

// Expose setupUI to global scope for auth.js
window.setupUI = setupUI;