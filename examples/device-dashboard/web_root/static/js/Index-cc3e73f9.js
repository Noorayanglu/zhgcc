import{g as a}from"./user-5ec4b135.js";import{d as e,e as t,i as n,ag as s,c as i,a as o,F as m,a7 as l,u as d,U as u,O as r,az as c,aA as p,o as v,J as j,T as k,S as y}from"./@vue-9d7d117a.js";import{_ as g}from"./index-2668d274.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-efa01f16.js";import"./feedback-179fee83.js";import"./element-plus-1abe4ec3.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-a55f440f.js";import"./@element-plus-9c1020e5.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./nprogress-867e68f1.js";import"./vue-router-d0599d8d.js";import"./pinia-c89a5039.js";/* empty css                    */const b=[{name:"全程电终端",coordinate:[72,9],data:[{name:"储能状态",unit:""},{name:"储能功率",unit:"kW"},{name:"储能SOC",unit:"%"}]},{name:"1#",coordinate:[66,22],data:[{name:"储能状态",unit:""},{name:"储能功率",unit:"kW"},{name:"储能SOC",unit:"%"}]},{name:"光伏",coordinate:[58,64],data:[{name:"光伏功率",unit:"kW"}]},{name:"负载",coordinate:[58,49],data:[{name:"负载功率",unit:"kW"}]},{name:"关口表",coordinate:[58,78],data:[{name:"关口表功率",unit:"kW"}]}],f=[{name:"ac1",coordinate:[32,34],data:[{name:"室外",key:"空调室外温度",unit:"℃"},{name:"出水",key:"空调出水温度",unit:"℃"},{name:"回水",key:"空调回水温度",unit:"℃"}]},{name:"dh1",coordinate:[24,45],data:[{name:"温度",key:"除湿机温度",unit:"℃"},{name:"湿度",key:"除湿机湿度",unit:"%"}]},{name:"pcs1",coordinate:[55,31],data:[{name:"有功功率",key:"有功功率",unit:"kW"},{name:"IGBT温度",key:"IGBT温度",unit:"℃"},{name:"AB相线",key:"AB相线",unit:"V"},{name:"BC相线",key:"BC相线",unit:"V"},{name:"CA相线",key:"CA相线",unit:"V"}]},{name:"bms1",coordinate:[66,57],data:[{name:"直流高压状态",key:"直流高压状态",unit:""},{name:"总电压",key:"总电压",unit:"V"},{name:"总电流",key:"总电流",unit:"A"}]}],h=a=>(c("data-v-e54aad53"),a=a(),p(),a),x={class:"dashboard"},A=h((()=>o("div",{class:"bg-box"},[o("img",{src:"/static/jpg/bgQcd-392a3536.jpg",alt:""})],-1))),B={class:"tip-name"},C={class:"tip-value-box"},V=h((()=>o("div",null,[o("img",{src:"/static/png/ctn-0d0c1454.png",alt:""})],-1))),W={class:"ctn-name"},_={class:"ctn-value-box"},z=g(e({__name:"Index",setup(e){const c=b,p=f,g=t({}),h=t({}),z=t(!1);n((()=>{O()}));const I=()=>{z.value=!0},$=a=>{z.value=!1},O=()=>{a().then((a=>{g.value=a})).catch((a=>{}))};return(a,e)=>{const t=s("el-drawer");return v(),i("div",x,[A,o("div",null,[(v(!0),i(m,null,l(d(c),((a,e)=>(v(),i("div",{key:e,class:"tip-box",style:j(`top: ${a.coordinate[0]}%; left: ${a.coordinate[1]}%;`),onClick:I},[o("div",B,k(a.name),1),o("div",C,[(v(!0),i(m,null,l(a.data,((a,e)=>(v(),i("div",{key:e+"A",class:"tip-value"},[o("span",null,k(g.value?g.value[a.name]:""),1),y(" "+k(a.unit),1)])))),128))])],4)))),128))]),u(t,{modelValue:z.value,"onUpdate:modelValue":e[0]||(e[0]=a=>z.value=a),title:"1#",direction:"rtl","before-close":$,size:"70%"},{default:r((()=>[V,o("div",null,[(v(!0),i(m,null,l(d(p),((a,e)=>(v(),i("div",{key:e,class:"ctn-box",style:j(`top: ${a.coordinate[0]}%; left: ${a.coordinate[1]}%;`)},[o("div",W,k(a.name),1),o("div",_,[(v(!0),i(m,null,l(a.data,((a,e)=>(v(),i("div",{key:e+"A",class:"ctn-value"},[y(k(a.name)+"： ",1),o("span",null,k(h.value?h.value[a.key]:""),1),y(" "+k(a.unit),1)])))),128))])],4)))),128))])])),_:1},8,["modelValue"])])}}}),[["__scopeId","data-v-e54aad53"]]);export{z as default};