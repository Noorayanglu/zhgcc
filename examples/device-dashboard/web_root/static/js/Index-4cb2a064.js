import{e,f as a}from"./user-27607c29.js";import{j as l}from"./index-3bd673e3.js";import{g as s}from"./utils-e10d97cb.js";import{f as t}from"./feedback-179fee83.js";import{d as o,e as d,i as m,Y as r,Z as u,ag as i,c as n,U as p,O as c,o as v,a as f,T as j,F as b,a7 as g,M as _,u as h,S as x,az as y,aA as k}from"./@vue-9d7d117a.js";import{_ as w}from"./index-b268814a.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-efa01f16.js";import"./nprogress-867e68f1.js";import"./element-plus-1abe4ec3.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-a55f440f.js";import"./@element-plus-9c1020e5.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-d0599d8d.js";import"./pinia-c89a5039.js";/* empty css                    */const V=[{name:"停机",value:0},{name:"制冷",value:1},{name:"加热",value:2},{name:"自循环",value:3}],z=e=>(y("data-v-010dd51c"),e=e(),k(),e),I={class:"cooling"},C={class:"img-data"},U=z((()=>f("div",{class:"img-box"},[f("img",{src:"/static/png/cooling-2157892c.png",alt:""})],-1))),A={class:"imgdata-box"},F=z((()=>f("div",{class:"imgdata-name"},"通信状态",-1))),S={class:"imgdata-value"},B={class:"btn-box"},E={class:"card-box"},M=z((()=>f("div",{class:"card-header"},[f("span",null,"控制")],-1))),N={class:"tab-box"},O=w(o({__name:"Index",setup(o){const y=d([]),k=d({}),w=V;d(0);const z=d(""),O=d(null),R=d(2e3);m((()=>{q()})),r((()=>{Z()}));const T=d(),Y=u({mode:null,temp:null,setEnabled:!1}),Z=()=>{clearInterval(O.value),O.value=null},q=()=>{e().then((e=>{const{"通信状态":a,...t}=e;k.value=t,z.value=a,y.value=l(k.value),y.value.map((e=>(e.value=s(e.value),e))),Z(),O.value=setInterval((()=>{q()}),R.value)})).catch((e=>{}))};return(e,l)=>{const s=i("el-col"),o=i("el-row"),d=i("el-option"),m=i("el-select"),r=i("el-form-item"),u=i("el-input"),V=i("el-button"),O=i("el-form"),R=i("el-card"),Z=i("el-table-column"),q=i("el-table");return v(),n("div",I,[p(o,null,{default:c((()=>[p(s,{span:12},{default:c((()=>[f("div",C,[U,f("div",A,[p(o,null,{default:c((()=>[p(s,{span:24},{default:c((()=>[F])),_:1}),p(s,{span:24},{default:c((()=>[f("div",S,j(z.value),1)])),_:1})])),_:1})])]),f("div",B,[f("div",E,[p(R,{class:"box-card"},{header:c((()=>[M])),default:c((()=>[f("div",null,[p(O,{ref_key:"formRef",ref:T,model:Y,"label-width":"120px"},{default:c((()=>[p(r,{label:"模式",prop:"mode"},{default:c((()=>[p(m,{modelValue:Y.mode,"onUpdate:modelValue":l[0]||(l[0]=e=>Y.mode=e),placeholder:"请选择无功补偿方式"},{default:c((()=>[(v(!0),n(b,null,g(h(w),((e,a)=>(v(),_(d,{key:a,label:e.name,value:e.value},null,8,["label","value"])))),128))])),_:1},8,["modelValue"]),f("div",null," 当前温度设定："+j(k.value["温度设定"])+" ℃ ",1)])),_:1}),p(r,{label:"温度设定",prop:"temp"},{default:c((()=>[p(u,{modelValue:Y.temp,"onUpdate:modelValue":l[1]||(l[1]=e=>Y.temp=e),type:"number"},null,8,["modelValue"])])),_:1}),p(r,null,{default:c((()=>[p(V,{type:"primary",onClick:l[2]||(l[2]=e=>(e=>{if(!e)return;let l={mode:Y.mode,temp:Number(Y.temp)};a(l).then((e=>{t.msgSuccess("设置成功")})).catch((e=>{}))})(T.value))},{default:c((()=>[x("设置")])),_:1}),p(V,{onClick:l[3]||(l[3]=e=>{var a;(a=T.value)&&a.resetFields()})},{default:c((()=>[x("清空")])),_:1})])),_:1})])),_:1},8,["model"])])])),_:1})])])])),_:1}),p(s,{span:12},{default:c((()=>[f("div",N,[p(q,{data:y.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:c((()=>[p(Z,{type:"index",width:"60",align:"center",label:"序号"}),p(Z,{prop:"name",label:"数据名称",align:"center"}),p(Z,{prop:"value",label:"数值",align:"center"}),p(Z,{prop:"unit",label:"单位",align:"center",width:"80"})])),_:1},8,["data"])])])),_:1})])),_:1})])}}}),[["__scopeId","data-v-010dd51c"]]);export{O as default};
