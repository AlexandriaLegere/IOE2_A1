let url = 'https://io.adafruit.com/api/v2/***/feeds/***';
let counter = 0;
let bg, logo;
let imgs=[6]
let bgsize;
let ccs=["F44336","9C27B0","2196F3","009688","4CAF50","FF9800"]
let users=[]
let lastUpdate
function preload() {
    bg = loadImage('assets/Background.svg');
    logo = loadImage('assets/logo1.png');
    for (i=1;i<=6;i++){
        imgs[i-1]= loadImage('assets/' + i + '.png')
    }
}

function setup() {
    
    imageMode(CENTER);
    bgsize = createVector(0,0)
    noStroke();
    createCanvas(windowWidth, windowHeight);
    if (width<height/0.5625){
        bgsize.x=height/0.5625
        bgsize.y=height
    }else{
        bgsize.x=width
        bgsize.y=width*0.5625
    }
        users[0]= new UserButton(0,width/2-160,height-180,320);
        users[1]= new UserButton(1,130,230,60);
    users[2]= new UserButton(2,30,30,60);
        users[3]= new UserButton(3,130,230,60);
    users[4]= new UserButton(4,30,30,60);
        users[5]= new UserButton(5,130,230,60);
    randomizePositions();
}

function draw() {
    fill(255, 10);
    image(bg,width/2,bgsize.y/2,bgsize.x,bgsize.y)
    sideBar()
    
    if (counter % 180 == 0) {
        getData();
    }
    counter++;
    for (i=0;i<users.length;i++){
        users[i].display();
    }
}

function getData() {
    let data;
    httpGet(url, 'json', function (response) {
//        console.log(response);
//        data = response.last_value;
//        console.log(data);
        dataHandler(response);
    });
}

function sendData(turnOn){
  let postData ={
    "value": turnOn,
    "X-AIO-Key": "***"
  };
  httpPost(url + "/data", 'json', postData, function(result){
    console.log(result);
  });
}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
    if (width<height/0.5625){
        bgsize.x=height/0.5625
        bgsize.y=height
    }else{
        bgsize.x=width
        bgsize.y=width*0.5625
    }
    randomizePositions()
}

class UserButton{
    constructor(id,x,y,size){
        this.id=id;
        this.img="img"+id;
        this.x=x;
        this.y=y;
        this.size=size;
        this.boundary = new Boundary(x,y,size);
        this.active=false;
    }
    checkBounds(){
        return this.boundary.collides();
    }
    display(){
        let displaySize=this.size;
        if (this.checkBounds())
            displaySize+=10
        fill(color("#" + ccs[this.id]))
        if (this.active)
            ellipse(this.x+this.size/2,this.y+this.size/2, displaySize+10,displaySize+10);
        fill(200)
        ellipse(this.x+this.size/2,this.y+this.size/2, displaySize,displaySize);
        image(imgs[this.id],this.x+this.size/2,this.y+this.size/2, displaySize,displaySize)
    }
}

class Boundary{
    constructor(x,y,size){
        this.x1=x;
        this.y1=y;
        this.x2=x+size;
        this.y2=y+size;
    }
    collides(){
        if((mouseX > this.x1 && mouseX< this.x2) && (mouseY > this.y1 && mouseY< this.y2)){
            return true;
        }else{
            return false;
        }
    }
}

//Doesn't actualy do anything. It was just a way to see which user was being clicked if we needed it.
function mouseReleased(){
    checkAll();
}

function checkAll(){
    for (i=0;i<users.length;i++){
        if(users[i].checkBounds())
            return users[i].id
    }
}

function dataHandler(dat){
    console.log("Checking Date: " + dat.updated_at)
     if  (lastUpdate == dat.updated_at)
         return null;
    console.log("Checking Data Type: " +dat.last_value)
    lastUpdate = dat.updated_at;
    let value = "" + dat.last_value
    if (value.length!==1){
        setUsers(value)
    }else{
        updateUsers(value)
    }
//    value=parseInt(value)
   
}

function setUsers(dat){
    for(i=0;i<dat.length;i++){
        let char= dat.charAt(i);
        if (char==1)
            users[i].active=true
        if (char==0)
            users[i].active=false
    }
}

function sideBar(){
    fill(240,50);
//    beginShape();
//    vertex(0,0);
//    vertex(230,-40);
//    curveVertex(280, height/5);
//    curveVertex(200, height/1.9);
//    curveVertex(260, height/1.2);
//    vertex(230,height+50);
//    vertex(0,height);
//    endShape(CLOSE);
    image(logo,110,80, 174, 99);
//    ellipse(280, height/5,10)
//    ellipse(270, height/1.3,10)
}


function updateUsers(value){
    console.log("Modifying User" + value + "to" + !users[value].active +  "And Outputting New String")
    users[value].active= !users[value].active
    let userDataString = ""
    for(i=0;i<users.length;i++){
        if (users[i].active){
            userDataString+="1"
            }else{
                userDataString+="0"
            }
    }
    sendData(userDataString)
}

function randomizePositions(){
    for (i=1;i<users.length;i++){
        users[i].x=random(200,width-200);
        users[i].y=random(100,height-300);
        users[i].size=random(120,280);
        users[i].boundary = new Boundary(users[i].x,users[i].y,users[i].size);
    }
}