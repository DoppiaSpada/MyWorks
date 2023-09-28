function onload(){
    var menu = document.getElementsByClassName("menu")[0];
    var _menu = document.getElementsByClassName("_menu")[0];
    var no_menu = document.getElementsByClassName("no_menu")[0];
    var num = 0;

    menu.addEventListener("transitionrun", () => {
        if(num % 2 == 0){
            no_menu.style.display = "none";
            num++;
        }
        else{
            _menu.style.display = "none";
            num++
        }

        if(num > 1) num = 0;
    })

    menu.addEventListener("transitionend", () => {
        if(num % 2 != 0){
            _menu.style.display = "block";
        }
        else{
            no_menu.style.display = "block";
        }
    })
}