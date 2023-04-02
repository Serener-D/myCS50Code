function calculateAge() {
    var myBirthday = new Date('1997-04-15');
    var currentDay = new Date();
    const timeDifference = currentDay.getTime() - myBirthday.getTime();
    const dateDifference = new Date(timeDifference);
    return Math.abs(dateDifference.getUTCFullYear() - 1970);
}

var paragraph = document.getElementById("age");
paragraph.textContent = "I am " + calculateAge() + " years old.";