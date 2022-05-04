let firebaseConfig = {
	apiKey: "AIzaSyB8UXUD5zq6yjZgqRD9wo8CknEChQNhVX4",
	authDomain: "toggle-single-22dec.firebaseapp.com",
	databaseURL: "https://toggle-single-22dec-default-rtdb.firebaseio.com",
	projectId: "toggle-single-22dec",
	storageBucket: "toggle-single-22dec.appspot.com",
	messagingSenderId: "203456689229",
	appId: "1:203456689229:web:a0787c4731d63ce6729804",
	measurementId: "G-38WVE0QQHJ"
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

//set motor speed as zero initially
document.querySelector("#motorSpeed").value = 0;

function changeBackground(){
    var firebaseRef = firebase.database().ref("Motor").child("Status");
    var sw1_pos = Boolean;
    sw1_pos = document.querySelector("#switch-1").checked;

    if(sw1_pos == true){
        document.querySelector("#run-status").style.backgroundColor = "#E83A14";
        firebaseRef.set(1);
    } 
    if (sw1_pos == false){
        document.querySelector("#run-status").style.backgroundColor = "#6BCB77";
        firebaseRef.set(0);
    }
}

function  changeDirection(){
    var firebaseRef = firebase.database().ref("Motor").child("Direction");
    var sw1_pos = Boolean;
    sw1_pos = document.querySelector("#switch-2").checked;

    if(sw1_pos == true){
        document.querySelector("#direction").style.backgroundColor = "#6EDCD9";
        firebaseRef.set(1);
    } 
    if (sw1_pos == false){
        document.querySelector("#direction").style.backgroundColor = "#6BCB77";
        firebaseRef.set(0);
    }
}

function displaySpeed(){
    var firebaseRef = firebase.database().ref("Motor").child("Speed");

    var speed = parseInt(document.querySelector("#motorSpeed").value);
    document.querySelector("#speedLabel").textContent = speed;


    firebaseRef.set(speed);
}