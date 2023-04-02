function displayResult() {
    document.getElementById("result1").innerHTML = "Correct!";
    document.getElementById("rightAnswer").style.backgroundColor = 'green';
}

function displayWrongResult1() {
    document.getElementById("result1").innerHTML = "Incorrect";
    document.getElementById("wrongAnswer1").style.backgroundColor = 'red';
}

function displayWrongResult2() {
    document.getElementById("result1").innerHTML = "Incorrect";
    document.getElementById("wrongAnswer2").style.backgroundColor = 'red';
}

document.getElementById("wrongAnswer1").addEventListener("click", displayWrongResult1);
document.getElementById("wrongAnswer2").addEventListener("click", displayWrongResult2);
document.getElementById("rightAnswer").addEventListener("click", displayResult);


function checkUserResponse() {
    if (document.getElementById("userResponse").value == "Canberra") {
        document.getElementById("userResponse").style.backgroundColor = 'green';
        document.getElementById("result2").innerHTML = "Correct!";
    } else {
        document.getElementById("userResponse").style.backgroundColor = 'red';
        document.getElementById("result2").innerHTML = "Incorrect";
    }
}

document.getElementById("confirmButton").addEventListener("click", checkUserResponse);
