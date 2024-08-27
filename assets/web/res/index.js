/*
 *
 * Copyright (C) 2024 BigfootACA <bigfoot@classfun.cn>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

let data={
	base: location.origin,
	running: false,
	input_run: false,
	first_fullscreen: false,
	statusText: "Loading",
	outboundSize: 0,
	width: 640,
	height: 480,
	move_interval: 20,
	mouse: "absolute",
	last_report:{
		movementX: 0,
		movementY: 0,
		time: 0,
	}
};

function setHidden(obj,hidden){
	if(hidden)obj.classList.add("hidden");
	else obj.classList.remove("hidden");
}

async function setAnimateHidden(obj){
	return new Promise(resolve=>{
		obj.classList.add("animate-hidden");
		setTimeout(()=>{
			obj.classList.add("hidden");
			obj.classList.remove("animate-hidden");
			resolve();
		},1000);
	});
}

function moveElement(obj,x,y){
	obj.style.left=`${x}px`;
	obj.style.top=`${y}px`;
}

function calcBounding(x,y,self,parent){
	if(x<parent.left)x=0;
	if(y<parent.top)y=0;
	if(x+self.width>parent.right)x=parent.right-self.width;
	if(y+self.height>parent.bottom)y=parent.bottom-self.height;
	return {x:x,y:y};
}

function setScreenSize(width,height){
	data.screen.style.width=`${width}px`;
	data.screen.style.height=`${height}px`;
	data.canvas.width=width;
	data.canvas.height=height;
	data.video.width=width;
	data.video.height=height;
	data.draw.reset();
	setStatus(null);
}

function addOutbound(value){
	data.outboundSize+=value;
	document.body.querySelector("span#outbound").innerText=data.outboundSize;
}

function removeChildren(obj){
	while(obj.firstChild)obj.removeChild(obj.firstChild);
}

function debounce(callback,wait){
	let id=null;
	return (...args)=>{
		clearTimeout(id);
		id=window.setTimeout(()=>callback(...args),wait);
	};
}

function setStatus(value){
	if(value==null)value=data.statusText;
	else data.statusText=value;
	document.body.querySelector("span#status").innerText=value;
	if(!data.running){
		let m=data.draw.measureText(value);
		let x=(data.canvas.width-m.width)/2;
		let y=(data.canvas.height)/2;
		data.draw.reset();
		data.draw.fillStyle="white";
		data.draw.fillText(value,x,y);
	}
}

async function sleep(msec){
	return new Promise(resolve=>setTimeout(()=>resolve(),msec));
}

async function waitGatheringComplete(pc){
	return new Promise(resolve=>{
		if(pc.iceGatheringState==="complete")resolve();
		else pc.addEventListener("icegatheringstatechange",()=>{
			if(pc.iceGatheringState==="complete")resolve();
		});
	});
}

function disconnectWebRTC(){
	data.running=false;
	data.input_run=false;
	setHidden(document.querySelector("li#connect"),false);
	setHidden(document.querySelector("li#disconnect"),true);
	setStatus("Disconnected");
	data.canvas.classList.remove("no-cursor");
	if(data.pc!=null)try{data.pc.close();}catch(e){}
	if(data.input_ch!=null)try{data.input_ch.close();}catch(e){}
	data.pc=null;
	data.input_ch=null;
}

function sendEvent(event){
	if(data.event_ch&&data.event_ch.readyState==="open"){
		data.last_report.time=now;
		data.input_ch.send(JSON.stringify(event));
	}
}

function processEvent(event){
	console.log(`got event ${event.type}`);
	console.log(event);
	switch(event.type){
		case "resize":
			if(event.width<=0||event.height<=0)break;
			data.width=event.width;
			data.height=event.height;
			onScreenResize();
		break;
	}
}

function onDatachannel(ev){
	const lbl=ev.channel.label;
	console.debug(`webrtc got data channel ${lbl}`);
	switch(lbl){
		case "input":
			data.input_ch=ev.channel;
			data.input_ch.addEventListener("open",()=>{
				console.debug("webrtc input channel opened");
				data.input_run=true;
			});
			data.input_ch.addEventListener("close",()=>{
				console.debug("webrtc input channel closed");
				data.input_run=false;
			});
		break;
		case "event":
			data.event_ch=ev.channel;
			data.event_ch.addEventListener("message",ev=>{
				processEvent(JSON.parse(ev.data));
			});
		break;
	}
}

function createWebRTC(){
	setStatus("Initializing");
	data.pc=new RTCPeerConnection({bundlePolicy:"max-bundle"});
	data.pc.addEventListener("iceconnectionstatechange",()=>
		console.debug(`webrtc ice connection state changed to ${data.pc.iceConnectionState}`));
	data.pc.addEventListener("icegatheringstatechange",()=>
		console.debug(`webrtc oce gathering state changed to ${data.pc.iceGatheringState}`));
	data.pc.addEventListener("connectionstatechange",()=>
		console.debug(`webrtc oce gathering state changed to ${data.pc.connectionState}`));
	data.pc.addEventListener("signalingstatechange",()=>
		console.debug(`webrtc signaling state changed to ${data.pc.signalingState}`));
	data.pc.addEventListener("track",ev=>{
		data.video.srcObject=ev.streams[0];
		data.video.play();
	});
	data.pc.addEventListener("datachannel",onDatachannel);
	data.pc.addEventListener("connectionstatechange",()=>{
		switch(data.pc.connectionState){
			case "disconnected":
				disconnectWebRTC();
			break;
		}
	});
}

async function apiCall(path,body={},noerr=true){
	const json=JSON.stringify(body);
	let headers={"Content-Type":"application/json"};
	if("token" in data)headers["Authorization"]=`Bearer ${data.token}`;
	const opts={
		method:"POST",
		body:json,
		headers:headers,
	};
	const res=await fetch(data.base+path,opts);
	let failed=noerr&&res.status!==200;
	let response=null;
	try{response=await res.json();}
	catch(e){if(!failed)throw e;}
	let code=res.status;
	if(response&&!response.success){
		failed=true;
		if(response.code)code=response.code;
		if(response.message)throw new Error(response.message);
	}
	if(failed)throw new Error(`API call ${path} failed: ${code}`);
	return response;
}

async function handshakeWebRTC(){
	setStatus("Handshaking");
	console.debug("start get remote sdp");
	const new_conn=await apiCall("/api/webrtc/new");
	data.id=new_conn.id;
	const get_sdp=await apiCall("/api/webrtc/get_sdp",{id:data.id});
	console.debug("got webrtc remote sdp");
	await data.pc.setRemoteDescription({type:"offer",sdp:get_sdp.sdp});
	await data.pc.setLocalDescription(await data.pc.createAnswer());
	console.debug("waiting gathering");
	setStatus("Gathering");
	await waitGatheringComplete(data.pc);
	console.debug("start set remote sdp");
	setStatus("Finishing");
	await apiCall("/api/webrtc/set_sdp",{
		id:data.id,
		sdp:data.pc.localDescription.sdp,
	});
	console.debug("webrtc handshake done");
	data.running=true;
	data.draw.reset();
	setStatus("Connected");
	data.canvas.classList.add("no-cursor");
}

async function startWebRTC(){
	try{
		setHidden(document.querySelector("li#connect"),true);
		setHidden(document.querySelector("li#disconnect"),true);
		createWebRTC();
		await handshakeWebRTC();
		setHidden(document.querySelector("li#disconnect"),false);
	}catch(e){
		disconnectWebRTC();
		console.error(e);
		setStatus("Failed");
		showMsgbox("Start WebRTC failed",`connect failed: ${e}`);
	}
}

function initializeFloatDrag(obj,cb=null,init_x=-1,init_y=-1){
	const parent=obj.parentElement;
	let base={x:0,y:0,w:0,h:0,m:false,c:false};
	let range=null,rect=null;
	obj.style.position="absolute";
	const bounded_move_to=(x,y)=>{
		let c=calcBounding(x,y,rect,range);
		moveElement(obj,c.x,c.y);
		localStorage.setItem(`${obj.id}-x`,c.x);
		localStorage.setItem(`${obj.id}-y`,c.y);
	}
	const moving=e=>{
		if(e.movementX===0&&e.movementY===0)return;
		base.m=true;
		let x=e.clientX-base.x;
		let y=e.clientY-base.y;
		bounded_move_to(x,y);
		if(cb)cb("mousemove",e);
	};
	obj.addEventListener("mousedown",e=>{
		range=parent.getBoundingClientRect();
		rect=obj.getBoundingClientRect();
		base.m=false;base.c=true;
		base.x=e.clientX-rect.x;
		base.y=e.clientY-rect.y;
		window.addEventListener("mousemove",moving,true);
		if(cb)cb("mousedown",e);
	});
	window.addEventListener("mouseup",e=>{
		if(!base.c)return;
		window.removeEventListener("mousemove",moving,true);
		if(cb)cb("mouseup",e);
		if(!base.m&&cb)cb("click",e);
		base.c=false;
	});
	new ResizeObserver(()=>{
		const old_range=range;
		range=parent.getBoundingClientRect();
		rect=obj.getBoundingClientRect();
		const new_x=rect.x*(range.width/old_range.width);
		const new_y=rect.y*(range.height/old_range.height);
		bounded_move_to(new_x,new_y);
		if(cb)cb("resize",null);
	}).observe(parent);
	range=parent.getBoundingClientRect();
	rect=obj.getBoundingClientRect();
	base.w=rect.width;
	base.h=rect.height;
	if(init_x===-1){
		init_x=localStorage.getItem(`${obj.id}-x`);
		if(init_x==null)init_x=(range.width-rect.width)/2;
		else init_x=parseInt(init_x);
	}
	if(init_y===-1){
		init_y=localStorage.getItem(`${obj.id}-y`);
		if(init_y==null)init_y=(range.height-rect.height)/2;
		else init_y=parseInt(init_y);
	}
	bounded_move_to(init_x,init_y);
}

function lockPointerRequest(){
	data.canvas.requestPointerLock();
}

function requestFullScreen(){
	document.body.requestFullscreen();
	document.querySelector("li#scale-fit").click();
	if(data.mouse==="relative")lockPointerRequest();
}

function exitFullScreen(){
	if(document.fullscreenElement!==null)
		document.exitFullscreen();
}

function keyCodeMap(code){
	return data.mapping===undefined?0:data.mapping[code];
}

function onInputEvent(type,event){
	let now=Date.now();
	let want_report=true;
	let relative=document.pointerLockElement===data.canvas;
	let buffer=new Uint8Array(12);
	let set8=(off,val)=>buffer.set([val],off);
	let set16=(off,val)=>buffer.set([val>>0&0xFF,val>>8&0xFF],off);
	let proc_mouse=(relative_type,absolute_type)=>{
		if(relative){
			data.last_report.movementX+=event.movementX;
			data.last_report.movementY+=event.movementY;
			set8(3,relative_type);
			set16(4,data.last_report.movementX);
			set16(6,data.last_report.movementY);
		}else{
			set8(3,absolute_type);
			set16(4,event.offsetX*65535/data.canvas.width);
			set16(6,event.offsetY*65535/data.canvas.height);
		}
		if(type.endsWith("down")||type.endsWith("up")){
			set8(8,event.button+1);
		}else if(now-data.last_report.time<data.move_interval)
			want_report=false;
		if(want_report){
			data.last_report.movementX=0;
			data.last_report.movementY=0;
		}
		event.preventDefault();
	};
	let proc_key=key_type=>{
		set8(3,key_type);
		set16(4,keyCodeMap(event.code));
	};
	let proc_wheel=wheel_type=>{
		set8(3,wheel_type);
		set16(4,event.deltaX);
		set16(6,event.deltaY);
		set16(8,event.deltaZ);
		event.preventDefault();
	};
	let proc_touch=touch_type=>{
		set8(3,touch_type);
		/* TODO */
		event.preventDefault();
	};
	buffer.set([0x21,0x49,0x4e],0); /* add magic head */
	switch(type){
		case "keydown":     proc_key(1);break;     /* EVENT_KEY_DOWN */
		case "keyup":       proc_key(2);break;     /* EVENT_KEY_UP */
		case "wheel":       proc_wheel(3);break;   /* EVENT_MOUSE_WHEEL */
		case "mousedown":   proc_mouse(7,4);break; /* EVENT_RELATIVE_DOWN EVENT_ABSOLUTE_DOWN */
		case "mouseup":     proc_mouse(8,5);break; /* EVENT_RELATIVE_UP   EVENT_ABSOLUTE_UP */
		case "mousemove":   proc_mouse(9,6);break; /* EVENT_RELATIVE_MOVE EVENT_ABSOLUTE_MOVE */
		case "touchstart":  proc_touch(10);break;  /* EVENT_TOUCH_START */
		case "touchend":    proc_touch(12);break;  /* EVENT_TOUCH_END */
		case "touchcancel": proc_touch(13);break;  /* EVENT_TOUCH_CANCEL */
		case "touchmove":   proc_touch(11);break;  /* EVENT_TOUCH_MOVE */
		default:return;
	}
	if(want_report&&data.input_run&&data.input_ch.readyState==="open"){
		data.last_report.time=now;
		data.input_ch.send(buffer);
		addOutbound(buffer.length);
	}
}

function initializeInput(){
	let add_listener=(obj,type)=>obj.addEventListener(type,event=>onInputEvent(type,event));
	add_listener(data.canvas,"mousemove");
	add_listener(data.canvas,"mousedown");
	add_listener(data.canvas,"mouseup");
	add_listener(data.canvas,"wheel");
	add_listener(data.canvas,"touchstart");
	add_listener(data.canvas,"touchend");
	add_listener(data.canvas,"touchcancel");
	add_listener(data.canvas,"touchmove");
	add_listener(document,"keydown");
	add_listener(document,"keyup");
}

function doCheckFunctions(){
	if(!("RTCPeerConnection" in window))
		throw new Error("no RTCPeerConnection for WebRTC");
}

async function showMsgbox(title,message,buttons={close:{title:"Close"}}){
	document.querySelector("div#msgbox-title").innerText=title;
	document.querySelector("div#msgbox-content").innerText=message;
	if(!("events" in data.msgbox)){
		data.msgbox.events=[];
		const handle_event=(type,event)=>{
			for(let i in data.msgbox.events)
				data.msgbox.events[i](type,event);
			return false;
		};
		data.msgbox.listen("MDCDialog:closing",event=>handle_event("closing",event));
		data.msgbox.listen("MDCDialog:closed",event=>handle_event("closed",event));
	}
	const btns=document.querySelector("div#msgbox-btns");
	removeChildren(btns);
	for(let action in buttons){
		const item=buttons[action];
		const btn=document.createElement("button");
		btn.classList.add("mdc-button");
		btn.classList.add("mdc-dialog__button");
		btn.setAttribute("data-mdc-dialog-action",action);
		const ripple_div=document.createElement("div");
		btn.appendChild(ripple_div);
		const label_span=document.createElement("span");
		label_span.innerText=item.title;
		btn.appendChild(label_span);
		btns.appendChild(btn);
		new mdc.ripple.MDCRipple(ripple_div);
	}

	data.msgbox.open();
	return new Promise(resolve=>{
		const id=data.msgbox.events.length;
		data.msgbox.events[id]=(type,event)=>{
			let action=null;
			try{
				action=event.detail.action;
				if(action in buttons){
					const item=buttons[action];
					if(type in item)item["type"]();
				}
			}catch(e){console.log(e);}
			if(type==="closed"){
				data.msgbox.events.splice(id,1);
				resolve(action);
			}
		};
	});
}

async function doRealLogin(){
	const payload={
		username:data.username.value,
		password:data.password.value,
	};
	const ret=await apiCall("/api/auth/login",payload);
	if(!ret.token)throw Error("no token in login response");
	data.token=ret.token;
	localStorage.setItem("token",data.token);
}

function doLogin(){
	const changeStatus=s=>{
		data.username.disabled=!s;
		data.password.disabled=!s;
		document.querySelector("button#login").disabled=!s;
	};
	changeStatus(false);
	doRealLogin().then(()=>{
		const mask=document.querySelector("div#login-mask");
		setAnimateHidden(mask).then(()=>{
			changeStatus(true);
		});
	}).catch(e=>{
		console.error(e);
		showMsgbox(
			"Login failed",
			e.message
		).then(()=>changeStatus(true));
	});
	return false;
}

async function setupKeyMap(){
	const ret=await apiCall("/api/input/get_key_map");
	data.mapping=ret.map;
}

async function setupLogin(){
	const ret=await apiCall("/api/auth/info");
	if(ret.no_login){
		setHidden(document.querySelector("div#login-mask"),true);
		setHidden(document.querySelector("li#logout"),true);
	}
	if(ret.hide_username)setHidden(document.querySelector("label#box-username"),true);
	if(ret.username.length!==0)data.username.value=ret.username;
	let token=localStorage.getItem("token");
	if(token!==null)try{
		const payload={token:token};
		await apiCall("/api/auth/check",payload);
		setHidden(document.querySelector("div#login-mask"),true);
		data.token=token;
	}catch(e){
		console.error(e);
		localStorage.removeItem("token");
	}
}

function doLogout(){
	localStorage.removeItem("token");
	data.token=null;
	window.location.reload();
}

async function loadPage(){
	await sleep(1000);
	initializeInput();
	onScreenResize();
	setStatus("Ready");
	doCheckFunctions();
	await setupKeyMap();
	await setupLogin();
	const mask=document.querySelector("div#loading-mask");
	setAnimateHidden(mask).then(()=>{
		data.page_loading.close();
	});
}

function initializeFloatButton(){
	const fab=document.querySelector("div#float-btn");
	const range=fab.parentElement.getBoundingClientRect();
	let init_x=localStorage.getItem(`${fab.id}-x`);
	let init_y=localStorage.getItem(`${fab.id}-y`);
	if(init_x==null)init_x=range.left+10;
	else init_x=parseInt(init_x);
	if(init_y==null)init_y=range.top+10;
	else init_y=parseInt(init_y);
	const move_menu=()=>{
		const rect=fab.getBoundingClientRect();
		let x=rect.x,y=rect.top+rect.height+10;
		data.menu.setAbsolutePosition(x,y);
	};
	const onevent=(type)=>{
		switch(type){
			case "click":
				data.menu.open=!data.menu.open;
				if(data.menu.open)move_menu();
			break;
			case "resize":
			case "mousemove":
				data.menu.open=false;
			break;
		}
	};
	initializeFloatDrag(fab,onevent,init_x,init_y);
}

function onFullScreenChanged(fullscreen){
	setHidden(document.querySelector("li#fullscreen"),fullscreen);
	setHidden(document.querySelector("li#fullscreen-exit"),!fullscreen);
}

function onScreenResize(){
	const scale_mode=document.querySelector("ul#radio-scale-mode");
	const fit=scale_mode.dataset.selected==="scale-fit";
	const par_rect=data.screen.parentElement.getBoundingClientRect();
	const scr=window.screen;
	const fw=par_rect.width,fh=par_rect.height;
	let fullscreen=document.fullscreenElement!==null;
	if(scr.height!==fh||scr.width!==fw)
		scr.first_fullscreen=false;
	else if(!scr.first_fullscreen){
		scr.first_fullscreen=true;
		fullscreen=true;
		document.querySelector("li#scale-fit").click();
	}
	onFullScreenChanged(fullscreen);
	let w=data.width,h=data.height;
	if(fit||fullscreen||w>fw||h>fh){
		let xw=fw,xh=xw*(h/w);
		if(xh>fh){xh=fh;xw=xh*(w/h);}
		w=Math.ceil(xw);h=Math.ceil(xh);
	}
	const x=(fw-w)/2+par_rect.x;
	const y=(fh-h)/2+par_rect.y;
	setScreenSize(w,h);
	moveElement(data.screen,x,y);
}

function initializeRadioMenu(){
	const icon_checked="radio_button_checked";
	const icon_unchecked="radio_button_unchecked";
	const set_checked=(obj,checked)=>{
		const status=obj.querySelector("span.mdc-radio-status");
		status.innerText=checked?icon_checked:icon_unchecked;
	};
	const is_checked=(obj)=>{
		const status=obj.querySelector("span.mdc-radio-status");
		if(status.innerText===icon_checked)return true;
		if(status.innerText===icon_unchecked)return false;
		set_checked(obj,false);
		return false;
	}
	const event=new Event("selectionchanged");
	document.querySelectorAll("ul.mdc-menu-radio").forEach(menu=>{
		const btns=menu.querySelectorAll("li.mdc-list-item");
		const expander=menu.parentElement.querySelector(`div.mdc-list-group__subheader[data-target='${menu.id}']`);
		const presistent=menu.dataset.presistent&&menu.dataset.presistent==="true";
		const storage=`${menu.id}-selected`;
		let def=null;
		if(presistent)def=localStorage.getItem(storage);
		const expander_set_open=opened=>expander.dataset.status=opened?"open":"close";
		const expander_is_open=()=>{
			if(expander.dataset.status==="open")return true;
			if(expander.dataset.status==="close")return false;
			expander_set_open(false);
			return false;
		}
		if(expander){
			const icon=expander.querySelector("span.mdc-menu-radio-expand");
			const change_open=opened=>{
				let height=0;
				btns.forEach(btn=>height+=btn.clientHeight);
				menu.style.height=`${height}px`;
				expander_set_open(opened);
				if(opened){
					icon.innerText="keyboard_arrow_up";
					menu.classList.remove("mdc-list-hidden");
				}else{
					icon.innerText="keyboard_arrow_down";
					menu.classList.add("mdc-list-hidden");
				}
			};
			change_open(expander_is_open());
			expander.addEventListener("click",()=>change_open(!expander_is_open()));
		}
		btns.forEach(btn=>{
			const change_check=()=>{
				btns.forEach(btn=>set_checked(btn,false));
				set_checked(btn,true);
				menu.dataset.selected=btn.id;
				if(presistent)localStorage.setItem(storage,btn.id);
			};
			btn.addEventListener("click",()=>{
				if(expander&&!expander_is_open())return;
				change_check();
				menu.dispatchEvent(event);
			});
			if(storage&&def!==null&&def===btn.id)change_check();
			else if(is_checked(btn))change_check();
		});
	});
}

function initializeScreenResizer(){
	new ResizeObserver(onScreenResize).observe(data.screen.parentElement);
	const scale_mode=document.querySelector("ul#radio-scale-mode");
	scale_mode.addEventListener("selectionchanged",onScreenResize);
	onScreenResize();
}

function initializeFullScreen(){
	document.addEventListener("keydown",(ev)=>{
		if(ev.code==="F11"){
			requestFullScreen();
			ev.preventDefault();
			return false;
		}
	})
	document.querySelector("li#fullscreen").addEventListener("click",requestFullScreen);
	document.querySelector("li#fullscreen-exit").addEventListener("click",exitFullScreen);
}

function initializeScreen(){
	data.screen=document.querySelector("div#screen");
	data.canvas=document.querySelector("div#screen canvas");
	data.video=document.querySelector("div#screen video");
	data.draw=data.canvas.getContext("2d");
	data.screen.oncontextmenu=()=>false;
	initializeScreenResizer();
}

function initializeLogin(){
	data.username=new mdc.textField.MDCTextField(document.querySelector("label#box-username"));
	data.password=new mdc.textField.MDCTextField(document.querySelector("label#box-password"));
	data.login=new mdc.ripple.MDCRipple(document.querySelector("button#login"));
	document.querySelector("form#login-form").onsubmit=doLogin;
}

function checkUSB(){
	if(!("USB" in window))
		throw Error("No WebUSB found (did you access in HTTPS?)");
}

function showUSBPage(){
	try{
		checkUSB();
		throw Error("USB Redirect unimplemented");
	}catch(e){
		showMsgbox("Load WebUSB failed",e.message);
	}
}

function showRemoteStoragePage(){
	try{
		throw Error("Remote storage unimplemented");
	}catch(e){
		showMsgbox("Load remote storage failed",e.message);
	}
}

function initializeMenu(){
	document.querySelector("li#usb-redirect").addEventListener("click",showUSBPage);
	document.querySelector("li#remote-storage").addEventListener("click",showRemoteStoragePage);
	document.querySelector("li#connect").addEventListener("click",startWebRTC);
	document.querySelector("li#disconnect").addEventListener("click",disconnectWebRTC);
	document.querySelector("li#logout").addEventListener("click",doLogout);
	setHidden(document.querySelector("li#connect"),false);
	setHidden(document.querySelector("li#disconnect"),true);
}

function initializeMDC(){
	const loading=document.querySelector("div#page-loading");
	loading.classList.remove("hidden");
	data.page_loading=new mdc.circularProgress.MDCCircularProgress(loading);
	data.page_loading.determinate=false;
	data.msgbox=new mdc.dialog.MDCDialog(document.querySelector("div#msgbox-dialog"));
	data.menu=new mdc.menu.MDCMenu(document.querySelector('div#float-menu'))
}

async function initializePage(){
	try{
		initializeMDC();
		initializeLogin();
		initializeScreen();
		initializeFullScreen();
		initializeFloatButton();
		initializeRadioMenu();
		initializeMenu();
		await loadPage();
	}catch(e){
		console.error(e);
		data.page_loading.close();
		document.querySelector("span#page-loading-status").innerText="Failed";
		showMsgbox("Page load failed",e.message);
	}
}

window.addEventListener("load",initializePage);
