import { initializeApp } from "https://www.gstatic.com/firebasejs/11.6.0/firebase-app.js";
import { getAuth } from "https://www.gstatic.com/firebasejs/11.6.0/firebase-auth.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/11.6.0/firebase-database.js";

// Firebase configuration as provided
const firebaseConfig = {
  apiKey: "enter apiKey",
  authDomain: "enter authDomain",
  projectId: "enter projectId",
  storageBucket: "enter storageBucket",
  messagingSenderId: "enter messagingSenderId",
  appId: "enter appId"
};

// Initializeing Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth(app);
const database = getDatabase(app, "https://sensor-reading-6a414-default-rtdb.asia-southeast1.firebasedatabase.app/");

// Export auth for use in auth.js
export { auth };

// UI Elements
const loginElement = document.querySelector('#login-form');
const contentElement = document.querySelector("#content-sign-in");
const userDetailsElement = document.querySelector('#user-details');
const authBarElement = document.querySelector("#authentication-bar");
const distElement = document.getElementById("dist");

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
   const dbPathdist = `UsersData/${uid}/dist`;

    // Database references
    const dbRefdist = ref(database, dbPathdist);

    // Update page with new readings
    onValue(dbRefdist, (snap) => {
  const val = parseFloat(snap.val());
  distElement.innerText = isNaN(val) ? "N/A" : val.toFixed(2);
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
