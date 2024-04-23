import{g as a,j as e,a as l,b as t}from"./index-34ffd1a0.js";import{g as s}from"./utils-e10d97cb.js";import{d as n,e as u,i,Y as o,ag as v,c,a as m,F as d,a7 as r,u as p,U as j,O as b,az as g,aA as h,o as x,J as y,T as f,S as k}from"./@vue-c5a747fc.js";import{_ as w}from"./index-2f62280c.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./feedback-551b2bff.js";import"./element-plus-a5ae5727.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const A=[{name:"全程电终端",coordinate:[66,4],data:[{name:"储能状态",unit:""},{name:"储能功率",unit:"kW"},{name:"储能SOC",unit:"%"}]},{name:"1#",coordinate:[62,22],data:[{name:"储能状态",unit:""},{name:"储能功率",unit:"kW"},{name:"储能SOC",unit:"%"}]},{name:"光伏",coordinate:[56,64],data:[{name:"光伏功率",unit:"kW"}]},{name:"负载",coordinate:[56,49],data:[{name:"负载功率",unit:"kW"}]},{name:"关口表",coordinate:[56,78],data:[{name:"关口表功率",unit:"kW"}]}],C=[{name:"AC液冷空调",coordinate:[25,17]},{name:"PCS变流器",coordinate:[55,16]},{name:"BMS电池管理系统",coordinate:[56,63]}],B=a=>(g("data-v-9657d12a"),a=a(),h(),a),I={class:"dashboard"},_=B((()=>m("div",{class:"bg-box"},[m("img",{src:"/static/jpg/bgQcd-392a3536.jpg",alt:""})],-1))),z={class:"tip-name"},S={class:"tip-value-box"},W={class:"ctndata-box-box"},O=B((()=>m("div",null,[m("img",{src:"/static/png/ctn-0d0c1454.png",alt:""})],-1))),T={class:"ctndata-box"},V={class:"ctn-name"},G={class:"ctn-value-box"},U=w(n({__name:"Index",setup(n){const g=A,h=C,w=u({}),B=u([]),U=u(!1),F=u({}),J=u({}),M=u([]),P=u({}),Q=u({}),Y=u([]),q=u({}),D=u({}),E=u([]),H=u(null),K=u(2e3);i((()=>{})),o((()=>{L()}));const L=()=>{clearInterval(H.value),H.value=null},N=async()=>{U.value=!0,await X(),L(),H.value=setInterval((()=>{X()}),K.value)},R=a=>{U.value=!1,L()},X=async()=>{B.value=[];let a=await aa(),e=await $(),l=await Z();B.value.push(l,a,e)},Z=async()=>await a().then((a=>(q.value=a,D.value={"工作模式":q.value["工作模式"],"出水温度":q.value["出水温度"],"回水温度":q.value["回水温度"],"出水压力":q.value["出水压力"],"回水压力":q.value["回水压力"]},E.value=e(D.value),E.value.map((a=>(a.value=s(a.value),a))),E.value))).catch((a=>{})),$=async()=>await l().then((a=>(F.value=a,J.value={"总电压":F.value["总电压"],"总电流":F.value["总电流"],"最高电芯电压":F.value["最高电芯电压"],"平均电芯电压":F.value["平均电芯电压"],"最低电芯电压":F.value["最低电芯电压"],"最高电芯温度":F.value["最高电芯温度"],"平均电芯温度":F.value["平均电芯温度"],"最低电芯温度":F.value["最低电芯温度"]},M.value=e(J.value),M.value.map((a=>(a.value=s(a.value),a))),M.value))).catch((a=>{})),aa=async()=>await t().then((a=>(P.value=a,Q.value={"有功功率":P.value["有功功率"],"IGBT温度":P.value["IGBT温度"],"AB线电压":P.value["AB线电压"],"BC线电压":P.value["BC线电压"],"CA线电压":P.value["CA线电压"]},Y.value=e(Q.value),Y.value.map((a=>(a.value=s(a.value),a))),Y.value))).catch((a=>{}));return(a,e)=>{const l=v("el-drawer");return x(),c("div",I,[_,m("div",null,[(x(!0),c(d,null,r(p(g),((a,e)=>(x(),c("div",{key:e,class:"tip-box",style:y("top: ".concat(a.coordinate[0],"%; left: ").concat(a.coordinate[1],"%;")),onClick:N},[m("div",z,f(a.name),1),m("div",S,[(x(!0),c(d,null,r(a.data,((a,e)=>(x(),c("div",{key:e+"A",class:"tip-value"},[m("span",null,f(w.value?w.value[a.name]:""),1),k(" "+f(a.unit),1)])))),128))])],4)))),128))]),j(l,{modelValue:U.value,"onUpdate:modelValue":e[0]||(e[0]=a=>U.value=a),title:"1#",direction:"rtl","before-close":R,size:"716"},{default:b((()=>[m("div",W,[O,m("div",T,[(x(!0),c(d,null,r(B.value,((a,e)=>(x(),c("div",{key:e,class:"ctn-box",style:y("top: ".concat(p(h)[e].coordinate[0],"%; left: ").concat(p(h)[e].coordinate[1],"%;"))},[m("div",V,f(p(h)[e].name),1),m("div",G,[(x(!0),c(d,null,r(a,((a,e)=>(x(),c("div",{key:e+"A",class:"ctn-value"},[k(f(a.name)+"： ",1),m("span",null,f(a.value),1),k(" "+f(a.unit),1)])))),128))])],4)))),128))])])])),_:1},8,["modelValue"])])}}}),[["__scopeId","data-v-9657d12a"]]);export{U as default};