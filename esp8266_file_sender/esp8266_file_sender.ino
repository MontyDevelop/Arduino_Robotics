#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

// ---------------- Settings you can change ----------------
const char* AP_SSID     = "ESP8266-FileSender";
const char* AP_PASSWORD = "MASTER123";   // 8+ chars, or "" for open network
const byte  DNS_PORT    = 53;
// -----------------------------------------------------------

DNSServer dnsServer;
ESP8266WebServer server(80);
IPAddress apIP(192, 168, 4, 1);

// Tiny "mailboxes" used only to pass the WebRTC handshake text
// between the two phones. Never holds the actual file.
String offerSDP  = "";
String answerSDP = "";

// ---------------- Web page ----------------
const char PAGE[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html><head>
<meta charset='utf-8'>
<meta name='viewport' content='width=device-width, initial-scale=1'>
<title>File Transfer</title>
<style>
  :root{
    --bg:#f4f4f6;
    --surface:#ffffff;
    --ink:#111111;
    --muted:#8a8f98;
    --line:#111111;
    --pill:#111111;
    --pill-ink:#ffffff;
    --track:#e6e6e9;
  }
  *{box-sizing:border-box;}
  body{
    margin:0; min-height:100vh; background:var(--bg); color:var(--ink);
    font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Helvetica,Arial,sans-serif;
    display:flex; justify-content:center; padding:36px 18px 60px;
  }
  .wrap{ width:100%; max-width:420px; }
  .eyebrow{
    text-align:center; color:var(--muted); font-size:12px;
    letter-spacing:.12em; text-transform:uppercase; margin-bottom:8px;
  }
  h1{
    text-align:center; font-size:21px; font-weight:700; margin:0 0 34px;
    letter-spacing:-.01em;
  }
  .hidden{ display:none !important; }

  /* ---- Role select cards ---- */
  .role-row{ display:flex; gap:14px; margin-bottom:26px; }
  .role-card{
    flex:1; background:var(--surface); border:2px solid var(--line);
    border-radius:22px; padding:30px 14px; text-align:center;
    font-weight:700; font-size:16px; line-height:1.35; cursor:pointer;
    box-shadow:0 20px 32px -14px rgba(17,17,17,.45);
    transition:transform .15s ease;
    -webkit-tap-highlight-color:transparent;
  }
  .role-card:active{ transform:scale(.96); }
  .role-card .sub{
    display:block; margin-top:8px; font-size:12px; font-weight:500;
    color:var(--muted); text-transform:none; letter-spacing:0;
  }

  /* ---- Generic card ---- */
  .card{
    background:var(--surface); border:2px solid var(--line); border-radius:22px;
    padding:24px 20px; margin-bottom:16px;
    box-shadow:0 20px 32px -14px rgba(17,17,17,.45);
  }
  .card h3{ margin:0 0 16px; font-size:15px; font-weight:700; }
  .label{ font-size:13px; color:var(--muted); margin:0 0 12px; }

  /* ---- Buttons ---- */
  .pill-btn{
    display:block; width:100%; text-align:center; appearance:none; border:none;
    background:var(--pill); color:var(--pill-ink); font-weight:700; font-size:15px;
    padding:16px; border-radius:999px; cursor:pointer; text-decoration:none;
    margin-bottom:12px; transition:transform .15s ease, opacity .15s ease;
    -webkit-tap-highlight-color:transparent;
  }
  .pill-btn:active{ transform:scale(.97); }
  .pill-btn[disabled]{ opacity:.35; pointer-events:none; }
  .pill-btn.outline{
    background:var(--surface); color:var(--ink); border:2px solid var(--line);
  }
  .pill-btn:last-child{ margin-bottom:0; }
  .pill-btn:focus-visible, .role-card:focus-visible{
    outline:2px solid var(--ink); outline-offset:3px;
  }

  input[type=file]{ display:none; }
  .file-row{ display:flex; align-items:center; gap:12px; margin-bottom:16px; }
  .file-row .pill-btn{ width:auto; flex:0 0 auto; margin:0; padding:12px 20px; font-size:14px; }
  #fileName{ font-size:13px; color:var(--muted); overflow:hidden; text-overflow:ellipsis; white-space:nowrap; }

  /* ---- Progress ---- */
  .bar{ height:8px; background:var(--track); border-radius:999px; overflow:hidden; margin:6px 0 10px; }
  .fill{ height:100%; width:0%; background:var(--ink); border-radius:999px; transition:width .15s ease; }
  .status{ font-size:13px; color:var(--muted); margin:0 0 4px; min-height:18px; }

  /* ---- Receiver preview ---- */
  .preview-media{ display:block; width:100%; border-radius:16px; margin-bottom:10px; background:var(--track); }
  .hint{ font-size:12px; color:var(--muted); margin:0 0 16px; text-align:center; }
  .file-chip{
    display:flex; justify-content:space-between; align-items:center;
    background:var(--bg); border-radius:14px; padding:14px 16px; margin-bottom:16px; font-size:14px;
  }
  .file-chip span{ color:var(--muted); font-size:12px; }

  @media (prefers-reduced-motion: reduce){ *{ transition:none !important; } }
</style>
</head>
<body>
<div class='wrap'>

  <div id='roleView'>
    <p class='eyebrow'>Local Wi-Fi transfer</p>
    <h1>Send a file between two phones</h1>
    <div class='role-row'>
      <div class='role-card' id='btnSender' tabindex='0'>I am sending<span class='sub'>Choose &amp; send a file</span></div>
      <div class='role-card' id='btnReceiver' tabindex='0'>I am receiving<span class='sub'>Wait &amp; get the file</span></div>
    </div>
  </div>

  <div class='card hidden' id='senderView'>
    <h3>Send a file</h3>
    <p class='label'>Pick a file, then connect to the other phone.</p>
    <div class='file-row'>
      <label class='pill-btn outline' for='fileInput'>Choose file</label>
      <span id='fileName'>No file selected</span>
    </div>
    <input type='file' id='fileInput'>
    <div class='bar'><div class='fill' id='fill'></div></div>
    <p class='status' id='status'></p>
    <button class='pill-btn' id='sendBtn' disabled>Send</button>
    <button class='pill-btn outline' onclick='location.reload()'>Start over</button>
  </div>

  <div class='card hidden' id='receiverView'>
    <h3>Receive a file</h3>
    <div class='bar'><div class='fill' id='fill2'></div></div>
    <p class='status' id='status2'>Waiting for sender...</p>
    <div id='previewArea'></div>
    <div id='saveArea'></div>
    <button class='pill-btn outline' onclick='location.reload()'>Start over</button>
  </div>

</div>

<script>
const $=id=>document.getElementById(id);
function show(id){ ['roleView','senderView','receiverView'].forEach(v=>$(v).classList.toggle('hidden', v!==id)); }
function sleep(ms){ return new Promise(r=>setTimeout(r,ms)); }

function waitIceComplete(pc){
  return new Promise(resolve=>{
    if(pc.iceGatheringState==='complete'){ resolve(); return; }
    function chk(){ if(pc.iceGatheringState==='complete'){ pc.removeEventListener('icegatheringstatechange',chk); resolve(); } }
    pc.addEventListener('icegatheringstatechange',chk);
    setTimeout(resolve, 4000);
  });
}

/* ---------------- role select ---------------- */
$('btnSender').onclick=()=>show('senderView');
$('btnReceiver').onclick=()=>{ show('receiverView'); startReceiving(); };
$('btnSender').onkeydown=(e)=>{ if(e.key==='Enter') $('btnSender').click(); };
$('btnReceiver').onkeydown=(e)=>{ if(e.key==='Enter') $('btnReceiver').click(); };

/* ---------------- SENDER ---------------- */
$('fileInput').addEventListener('change', ()=>{
  const f=$('fileInput').files[0];
  $('fileName').textContent = f ? f.name : 'No file selected';
  $('sendBtn').disabled = !f;
});

$('sendBtn').onclick = async ()=>{
  const file = $('fileInput').files[0];
  if(!file) return;
  $('sendBtn').disabled = true;
  $('status').textContent='Preparing connection...';
  await fetch('/signal/reset');

  const pc = new RTCPeerConnection({iceServers:[]});
  const dc = pc.createDataChannel('file');
  dc.binaryType='arraybuffer';

  dc.onopen = ()=>{ $('status').textContent='Connected. Sending...'; sendFile(file,dc); };
  dc.onclose = ()=>{ if($('status').textContent.indexOf('Done')<0) $('status').textContent='Connection closed'; };

  const offer = await pc.createOffer();
  await pc.setLocalDescription(offer);
  await waitIceComplete(pc);

  await fetch('/signal/offer', {method:'POST', body: JSON.stringify(pc.localDescription)});
  $('status').textContent='Waiting for the other phone to connect...';

  let answer=null;
  while(!answer){
    await sleep(700);
    const r = await fetch('/signal/answer'); const t = await r.text();
    if(t && t!=='{}') answer = JSON.parse(t);
  }
  $('status').textContent='Found receiver, connecting...';
  await pc.setRemoteDescription(answer);
};

function sendFile(file, dc){
  const CHUNK=16384;
  let offset=0;
  dc.send(JSON.stringify({type:'meta', name:file.name, size:file.size, mime:file.type||''}));

  function next(){
    if(offset>=file.size){
      dc.send(JSON.stringify({type:'eof'}));
      $('status').textContent='Done. File sent.';
      $('fill').style.width='100%';
      return;
    }
    if(dc.bufferedAmount>1000000){ setTimeout(next,40); return; }
    const slice=file.slice(offset, offset+CHUNK);
    const reader=new FileReader();
    reader.onload=()=>{
      dc.send(reader.result);
      offset+=slice.size;
      const pct=Math.round(offset/file.size*100);
      $('fill').style.width=pct+'%';
      $('status').textContent='Sending '+pct+'%  ('+(offset/1048576).toFixed(1)+' / '+(file.size/1048576).toFixed(1)+' MB)';
      next();
    };
    reader.readAsArrayBuffer(slice);
  }
  next();
}

/* ---------------- RECEIVER ---------------- */
let recvMeta=null, recvChunks=[], recvSize=0;

async function startReceiving(){
  const pc = new RTCPeerConnection({iceServers:[]});
  pc.ondatachannel = (e)=>{
    const dc = e.channel; dc.binaryType='arraybuffer';
    dc.onmessage = (ev)=>{
      if(typeof ev.data === 'string'){
        const msg = JSON.parse(ev.data);
        if(msg.type==='meta'){ recvMeta=msg; recvChunks=[]; recvSize=0; $('status2').textContent='Receiving '+msg.name+'...'; }
        else if(msg.type==='eof'){ finishReceive(); }
      } else {
        recvChunks.push(ev.data);
        recvSize += ev.data.byteLength;
        if(recvMeta && recvMeta.size){
          const pct=Math.round(recvSize/recvMeta.size*100);
          $('fill2').style.width=pct+'%';
          $('status2').textContent='Receiving '+pct+'%  ('+(recvSize/1048576).toFixed(1)+' / '+(recvMeta.size/1048576).toFixed(1)+' MB)';
        }
      }
    };
  };

  $('status2').textContent='Waiting for sender...';
  let offer=null;
  while(!offer){
    await sleep(700);
    const r = await fetch('/signal/offer'); const t = await r.text();
    if(t && t!=='{}') offer = JSON.parse(t);
  }
  $('status2').textContent='Found sender, connecting...';
  await pc.setRemoteDescription(offer);
  const answer = await pc.createAnswer();
  await pc.setLocalDescription(answer);
  await waitIceComplete(pc);
  await fetch('/signal/answer', {method:'POST', body: JSON.stringify(pc.localDescription)});
  $('status2').textContent='Connected. Waiting for file...';
}

function getKind(meta){
  const mime=(meta.mime||'').toLowerCase();
  const name=(meta.name||'').toLowerCase();
  if(mime.indexOf('image/')===0 || /\.(jpe?g|png|gif|webp|bmp)$/.test(name)) return 'image';
  if(mime.indexOf('video/')===0 || /\.(mp4|mov|webm|mkv|avi)$/.test(name)) return 'video';
  if(mime.indexOf('audio/')===0 || /\.(mp3|wav|m4a|aac|ogg)$/.test(name)) return 'audio';
  return 'other';
}

function finishReceive(){
  const blob = new Blob(recvChunks, {type: recvMeta.mime || 'application/octet-stream'});
  const url = URL.createObjectURL(blob);

  $('status2').textContent='Received: '+recvMeta.name;
  $('fill2').style.width='100%';

  const kind=getKind(recvMeta);
  const preview=$('previewArea');
  preview.innerHTML='';

  if(kind==='image'){
    const img=document.createElement('img');
    img.src=url; img.className='preview-media';
    preview.appendChild(img);
    const hint=document.createElement('p'); hint.className='hint';
    hint.textContent='Press and hold the image to save it to this phone.';
    preview.appendChild(hint);
  } else if(kind==='video'){
    const vid=document.createElement('video');
    vid.src=url; vid.controls=true; vid.className='preview-media';
    preview.appendChild(vid);
    const hint=document.createElement('p'); hint.className='hint';
    hint.textContent='Press and hold the video to save it to this phone.';
    preview.appendChild(hint);
  } else if(kind==='audio'){
    const aud=document.createElement('audio');
    aud.src=url; aud.controls=true; aud.style.width='100%'; aud.style.marginBottom='14px';
    preview.appendChild(aud);
  } else {
    const chip=document.createElement('div'); chip.className='file-chip';
    const left=document.createElement('strong'); left.textContent=recvMeta.name;
    const right=document.createElement('span'); right.textContent=(recvMeta.size/1048576).toFixed(2)+' MB';
    chip.appendChild(left); chip.appendChild(right);
    preview.appendChild(chip);
  }

  const saveBtn=document.createElement('button');
  saveBtn.className='pill-btn';
  saveBtn.textContent='Save file';
  saveBtn.onclick=()=>{
    const a=document.createElement('a');
    a.href=url; a.download=recvMeta.name;
    document.body.appendChild(a); a.click(); document.body.removeChild(a);
  };

  const openBtn=document.createElement('a');
  openBtn.className='pill-btn outline';
  openBtn.textContent='Open in new tab';
  openBtn.href=url; openBtn.target='_blank'; openBtn.rel='noopener';

  const saveArea=$('saveArea');
  saveArea.innerHTML='';
  saveArea.appendChild(saveBtn);
  saveArea.appendChild(openBtn);
}
</script>
</body></html>
)HTML";

// ---------------- Handlers ----------------
void handleRoot() {
  server.send_P(200, "text/html", PAGE);
}

void handleOfferPost() {
  offerSDP = server.hasArg("plain") ? server.arg("plain") : "";
  answerSDP = ""; // a fresh offer means a fresh session
  server.send(200, "text/plain", "OK");
}
void handleOfferGet() {
  server.send(200, "application/json", offerSDP.length() ? offerSDP : "{}");
}
void handleAnswerPost() {
  answerSDP = server.hasArg("plain") ? server.arg("plain") : "";
  server.send(200, "text/plain", "OK");
}
void handleAnswerGet() {
  server.send(200, "application/json", answerSDP.length() ? answerSDP : "{}");
}
void handleReset() {
  offerSDP = "";
  answerSDP = "";
  server.send(200, "text/plain", "OK");
}

// Captive-portal probe URLs -> redirect to our page so phones
// auto-popup the "Sign in to network" screen.
void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  delay(300);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  Serial.println();
  Serial.println("Access Point started");
  Serial.print("SSID: "); Serial.println(AP_SSID);
  Serial.print("IP:   "); Serial.println(WiFi.softAPIP());

  dnsServer.start(DNS_PORT, "*", apIP);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/signal/offer",  HTTP_POST, handleOfferPost);
  server.on("/signal/offer",  HTTP_GET,  handleOfferGet);
  server.on("/signal/answer", HTTP_POST, handleAnswerPost);
  server.on("/signal/answer", HTTP_GET,  handleAnswerGet);
  server.on("/signal/reset",  HTTP_GET,  handleReset);

  server.on("/generate_204", handleNotFound);
  server.on("/hotspot-detect.html", handleNotFound);
  server.on("/connecttest.txt", handleNotFound);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}
