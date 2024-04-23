import{g as a,j as e,a as t,b as l}from"./index-d0db263c.js";import{g as s}from"./utils-e10d97cb.js";import{d as n,e as u,i,ag as o,c as v,a as c,F as m,a7 as d,u as r,U as p,O as j,az as f,aA as b,o as g,J as h,T as x,S as y}from"./@vue-c5a747fc.js";import{_ as k}from"./index-c5ad40e3.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./feedback-551b2bff.js";import"./element-plus-a5ae5727.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const A=[{name:"全程电终端",coordinate:[72,9],data:[{name:"储能状态",unit:""},{name:"储能功率",unit:"kW"},{name:"储能SOC",unit:"%"}]},{name:"1#",coordinate:[66,22],data:[{name:"储能状态",unit:""},{name:"储能功率",unit:"kW"},{name:"储能SOC",unit:"%"}]},{name:"光伏",coordinate:[58,64],data:[{name:"光伏功率",unit:"kW"}]},{name:"负载",coordinate:[58,49],data:[{name:"负载功率",unit:"kW"}]},{name:"关口表",coordinate:[58,78],data:[{name:"关口表功率",unit:"kW"}]}],C=[{name:"AC液冷空调",coordinate:[25,17]},{name:"PCS变流器",coordinate:[55,16]},{name:"BMS电池管理系统",coordinate:[56,63]}],w=a=>(f("data-v-7e06eff2"),a=a(),b(),a),B={class:"dashboard"},_=w((()=>c("div",{class:"bg-box"},[c("img",{src:"/static/jpg/bgQcd-392a3536.jpg",alt:""})],-1))),z={class:"tip-name"},S={class:"tip-value-box"},W={class:"ctndata-box-box"},I=w((()=>c("div",null,[c("img",{src:"/static/png/ctn-0d0c1454.png",alt:""})],-1))),O={class:"ctndata-box"},T={class:"ctn-name"},V={class:"ctn-value-box"},G=k(n({__name:"Index",setup(n){const f=A,b=C,k=u({}),w=u([]),G=u(!1),U=u({}),F=u({}),J=u([]),M=u({}),P=u({}),Q=u([]),q=u({}),D=u({}),E=u([]);i((()=>{}));const H=()=>{G.value=!0,L()},K=a=>{G.value=!1},L=async()=>{w.value=[];let a=await X(),e=await R(),t=await N();w.value.push(t,a,e)},N=async()=>await a().then((a=>(q.value=a,D.value={"工作模式":q.value["工作模式"],"出水温度":q.value["出水温度"],"回水温度":q.value["回水温度"],"出水压力":q.value["出水压力"],"回水压力":q.value["回水压力"]},E.value=e(D.value),E.value.map((a=>(a.value=s(a.value),a))),E.value))).catch((a=>{})),R=async()=>await t().then((a=>(U.value=a,F.value={"总电压":U.value["总电压"],"总电流":U.value["总电流"],"最高电芯电压":U.value["最高电芯电压"],"平均电芯电压":U.value["平均电芯电压"],"最低电芯电压":U.value["最低电芯电压"],"最高电芯温度":U.value["最高电芯温度"],"平均电芯温度":U.value["平均电芯温度"],"最低电芯温度":U.value["最低电芯温度"]},J.value=e(F.value),J.value.map((a=>(a.value=s(a.value),a))),J.value))).catch((a=>{})),X=async()=>await l().then((a=>(M.value=a,P.value={"有功功率":M.value["有功功率"],"IGBT温度":M.value["IGBT温度"],"AB线电压":M.value["AB线电压"],"BC线电压":M.value["BC线电压"],"CA线电压":M.value["CA线电压"]},Q.value=e(P.value),Q.value.map((a=>(a.value=s(a.value),a))),Q.value))).catch((a=>{}));return(a,e)=>{const t=o("el-drawer");return g(),v("div",B,[_,c("div",null,[(g(!0),v(m,null,d(r(f),((a,e)=>(g(),v("div",{key:e,class:"tip-box",style:h("top: ".concat(a.coordinate[0],"%; left: ").concat(a.coordinate[1],"%;")),onClick:H},[c("div",z,x(a.name),1),c("div",S,[(g(!0),v(m,null,d(a.data,((a,e)=>(g(),v("div",{key:e+"A",class:"tip-value"},[c("span",null,x(k.value?k.value[a.name]:""),1),y(" "+x(a.unit),1)])))),128))])],4)))),128))]),p(t,{modelValue:G.value,"onUpdate:modelValue":e[0]||(e[0]=a=>G.value=a),title:"1#",direction:"rtl","before-close":K,size:"716"},{default:j((()=>[c("div",W,[I,c("div",O,[(g(!0),v(m,null,d(w.value,((a,e)=>(g(),v("div",{key:e,class:"ctn-box",style:h("top: ".concat(r(b)[e].coordinate[0],"%; left: ").concat(r(b)[e].coordinate[1],"%;"))},[c("div",T,x(r(b)[e].name),1),c("div",V,[(g(!0),v(m,null,d(a,((a,e)=>(g(),v("div",{key:e+"A",class:"ctn-value"},[y(x(a.name)+"： ",1),c("span",null,x(a.value),1),y(" "+x(a.unit),1)])))),128))])],4)))),128))])])])),_:1},8,["modelValue"])])}}}),[["__scopeId","data-v-7e06eff2"]]);export{G as default};
