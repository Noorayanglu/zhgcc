System.register(["./@vue-legacy-55b7f08a.js","./index-legacy-cbf379d9.js","./vue-router-legacy-d2fecccb.js","./pinia-legacy-62dfefa8.js","./element-plus-legacy-8daa718a.js","./lodash-es-legacy-8e03b1de.js","./@vueuse-legacy-de216c25.js","./@element-plus-legacy-7308db8d.js","./@popperjs-legacy-876caf52.js","./@ctrl-legacy-33dbca08.js","./dayjs-legacy-d4cf0d68.js","./async-validator-legacy-aa1fd2de.js","./memoize-one-legacy-599ef04d.js","./escape-html-legacy-9d0f3640.js","./normalize-wheel-es-legacy-13d39621.js","./@floating-ui-legacy-82e00343.js","./animate.css-legacy-280ccc6e.js"],(function(e,n){"use strict";var l,u,t,a,r,c,i,o,d,f,s,m,p,g,y,v,h;return{setters:[function(e){l=e.d,u=e.Z,t=e.ag,a=e.c,r=e.U,c=e.O,i=e.o,o=e.F,d=e.a7,f=e.M,s=e.u,m=e.a,p=e.S,g=e.T,y=e.az,v=e.aA},function(e){h=e._},null,null,null,null,null,null,null,null,null,null,null,null,null,null,null],execute:function(){var n=document.createElement("style");n.textContent=".control .input-box[data-v-1373c8c2]{width:100%;display:flex;justify-content:space-between}.control .input-box .btn[data-v-1373c8c2]{margin-left:10px}.control .box-card[data-v-1373c8c2]{margin-left:20px;margin-bottom:20px}\n",document.head.appendChild(n);var _=[{name:"CC Current",unit:"A",key:"current",defaultVal:"140.000"},{name:"Charge Cell Voltage",unit:"V",key:"charge",defaultVal:"3.650"},{name:"Discharge Cell Voltage",unit:"V",key:"discharge",defaultVal:"2.500"},{name:"OCP",unit:"A",key:"ocp",defaultVal:"150.000"},{name:"Vh",unit:"V",key:"vh",defaultVal:"4.126"},{name:"Vl",unit:"V",key:"vl",defaultVal:"1.949"},{name:"OVP",unit:"V",key:"ovp",defaultVal:"4.128"},{name:"UVP",unit:"V",key:"uvp",defaultVal:"1.930"},{name:"Charge Idle Time",unit:"Min",key:"chargeIdleTime",defaultVal:"30"},{name:"Discharge Idle Time",unit:"Min",key:"dischargeIdleTime",defaultVal:"30"},{name:"Charge Time",unit:"",key:"chargeTime",defaultVal:"NA"},{name:"Discharge Time",unit:"",key:"dischargeTime",defaultVal:"NA"}],b=[{name:"None",value:0},{name:"CC Charge To Full",value:1},{name:"CC Discharge To Empty",value:2},{name:"CC Cycle",value:3},{name:"CC Pulse Charge To Full",value:4},{name:"CC Pulse Discharge To Empty",value:5}],V=function(e){return y("data-v-1373c8c2"),e=e(),v(),e},C={class:"control"},k={class:"input-box"},j=V((function(){return m("div",{class:"card-header"},[m("span",null,"State Machine")],-1)})),x=V((function(){return m("div",{class:""},null,-1)})),T=V((function(){return m("div",{class:""},null,-1)})),S=V((function(){return m("div",{class:""},null,-1)})),w=V((function(){return m("div",{class:""},[m("span",null,"0.000"),p(" s")],-1)})),I=V((function(){return m("div",{class:""},[m("span",null,"99.000"),p(" s")],-1)})),M=V((function(){return m("div",{class:""},[m("span",null,"0.000"),p(" s")],-1)})),A=V((function(){return m("div",{class:""},[m("span",null,"0.000")],-1)})),D=V((function(){return m("div",{class:"card-header"},[m("span",null,"Manual")],-1)}));e("default",h(l({__name:"Index",setup:function(e){var n=_,l=b,y=u({workMode:null,current:null,charge:null,discharge:null,ocp:null,vh:null,vl:null,ovp:null,uvp:null,chargeIdleTime:null,dischargeIdleTime:null,chargeTime:null,dischargeTime:null});return function(e,u){var v=t("el-option"),h=t("el-select"),_=t("el-form-item"),b=t("el-input"),V=t("el-button"),E=t("el-form"),P=t("el-col"),R=t("el-row"),U=t("el-card");return i(),a("div",C,[r(R,null,{default:c((function(){return[r(P,{lg:10,md:12},{default:c((function(){return[r(E,{model:y,"label-width":"120px"},{default:c((function(){return[r(_,{label:"Work Mode",prop:"nopower"},{default:c((function(){return[r(h,{modelValue:y.workMode,"onUpdate:modelValue":u[0]||(u[0]=function(e){return y.workMode=e})},{default:c((function(){return[(i(!0),a(o,null,d(s(l),(function(e,n){return i(),f(v,{key:n,label:e.name,value:e.value},null,8,["label","value"])})),128))]})),_:1},8,["modelValue"])]})),_:1}),(i(!0),a(o,null,d(s(n),(function(n,l){return i(),f(_,{key:l,label:n.name,prop:n.key},{default:c((function(){return[m("div",k,[r(b,{modelValue:y[n.key],"onUpdate:modelValue":function(e){return y[n.key]=e},type:"number"},{prepend:c((function(){return[p(g(n.defaultVal),1)]})),append:c((function(){return[p(g(n.unit),1)]})),_:2},1032,["modelValue","onUpdate:modelValue"]),m("div",null,[r(V,{class:"btn",type:"primary",onClick:u[1]||(u[1]=function(n){return e.onSubmit(e.formRef)})},{default:c((function(){return[p("设置")]})),_:1})])])]})),_:2},1032,["label","prop"])})),128))]})),_:1},8,["model"])]})),_:1}),r(P,{lg:7,md:12},{default:c((function(){return[r(U,{class:"box-card"},{header:c((function(){return[j]})),default:c((function(){return[m("div",null,[r(R,null,{default:c((function(){return[r(P,{span:16},{default:c((function(){return[r(E,{model:y,"label-width":"120px"},{default:c((function(){return[r(_,{label:"State："},{default:c((function(){return[x]})),_:1}),r(_,{label:"Step："},{default:c((function(){return[T]})),_:1}),r(_,{label:"Error："},{default:c((function(){return[S]})),_:1})]})),_:1},8,["model"])]})),_:1}),r(P,{span:8},{default:c((function(){return[r(V,{type:"primary",onClick:u[2]||(u[2]=function(n){return e.onSubmit(e.formRef)})},{default:c((function(){return[p("Start")]})),_:1})]})),_:1}),r(P,{span:16},{default:c((function(){return[r(E,{model:y,"label-width":"120px"},{default:c((function(){return[r(_,{label:"Max Intv："},{default:c((function(){return[w]})),_:1}),r(_,{label:"Min Intv："},{default:c((function(){return[I]})),_:1}),r(_,{label:"Avg Intv："},{default:c((function(){return[M]})),_:1}),r(_,{label:"Tot Cnt："},{default:c((function(){return[A]})),_:1})]})),_:1},8,["model"])]})),_:1}),r(P,{span:8},{default:c((function(){return[r(V,{type:"primary",onClick:u[3]||(u[3]=function(n){return e.onSubmit(e.formRef)})},{default:c((function(){return[p("Reset Statis")]})),_:1})]})),_:1})]})),_:1})])]})),_:1})]})),_:1}),r(P,{lg:7,md:12},{default:c((function(){return[r(U,{class:"box-card"},{header:c((function(){return[D]})),default:c((function(){return[m("div",null,[r(V,{type:"primary",onClick:u[4]||(u[4]=function(n){return e.onSubmit(e.formRef)})},{default:c((function(){return[p("Emergency Stop")]})),_:1})])]})),_:1})]})),_:1})]})),_:1})])}}}),[["__scopeId","data-v-1373c8c2"]]))}}}));
