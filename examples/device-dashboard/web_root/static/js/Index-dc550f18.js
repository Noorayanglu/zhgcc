import{g as e,j as a,d as l}from"./index-d0db263c.js";import{g as t}from"./utils-e10d97cb.js";import{f as s}from"./feedback-551b2bff.js";import{d as o,e as u,i as r,Y as d,Z as m,ag as n,c as i,U as p,O as c,o as v,a as f,J as b,T as j,F as g,a7 as _,M as h,u as x,S as y,az as w,aA as V}from"./@vue-c5a747fc.js";import{_ as k}from"./index-c5ad40e3.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-a5ae5727.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const C=[{name:"停机",value:0},{name:"制冷",value:1},{name:"加热",value:2},{name:"自循环",value:3}],z=e=>(w("data-v-708948fe"),e=e(),V(),e),I={class:"cooling"},A={class:"img-data"},F=z((()=>f("div",{class:"img-box"},[f("img",{src:"/static/png/cooling-2157892c.png",alt:""})],-1))),U={class:"imgdata-box"},E=z((()=>f("div",{class:"imgdata-name"},"通信状态",-1))),S={class:"btn-box"},B={class:"card-box"},J=z((()=>f("div",{class:"card-header"},[f("span",null,"控制")],-1))),M={class:"tab-box"},N=k(o({__name:"Index",setup(o){const w=u([]),V=u({}),k=C;u(0);const z=u(""),N=u(null),O=u(2e3);r((()=>{Z()})),d((()=>{Y()}));const R=u(),T=m({mode:null,temp:null,setEnabled:!1}),Y=()=>{clearInterval(N.value),N.value=null},Z=()=>{e().then((e=>{const{"通信状态":l,...s}=e;V.value=s,z.value=l,w.value=a(V.value),w.value.map((e=>(e.value=t(e.value),e))),Y(),N.value=setInterval((()=>{Z()}),O.value)})).catch((e=>{}))};return(e,a)=>{const t=n("el-col"),o=n("el-row"),u=n("el-option"),r=n("el-select"),d=n("el-form-item"),m=n("el-input"),C=n("el-button"),N=n("el-form"),O=n("el-card"),Y=n("el-table-column"),Z=n("el-table");return v(),i("div",I,[p(o,null,{default:c((()=>[p(t,{span:12},{default:c((()=>[f("div",A,[F,f("div",U,[p(o,null,{default:c((()=>[p(t,{span:24},{default:c((()=>[E])),_:1}),p(t,{span:24},{default:c((()=>[f("div",{class:"imgdata-value",style:b("color:".concat("正常"==z.value?"#67C23A":"#F56C6C",";"))},j(z.value),5)])),_:1})])),_:1})])]),f("div",S,[f("div",B,[p(O,{class:"box-card"},{header:c((()=>[J])),default:c((()=>[f("div",null,[p(N,{ref_key:"formRef",ref:R,model:T,"label-width":"80px"},{default:c((()=>[p(d,{label:"模式",prop:"mode"},{default:c((()=>[p(r,{modelValue:T.mode,"onUpdate:modelValue":a[0]||(a[0]=e=>T.mode=e),placeholder:"请选择模式"},{default:c((()=>[(v(!0),i(g,null,_(x(k),((e,a)=>(v(),h(u,{key:a,label:e.name,value:e.value},null,8,["label","value"])))),128))])),_:1},8,["modelValue"]),f("div",null," 当前温度设定："+j(V.value["温度设定"])+" ℃ ",1)])),_:1}),p(d,{label:"温度设定",prop:"temp"},{default:c((()=>[p(m,{modelValue:T.temp,"onUpdate:modelValue":a[1]||(a[1]=e=>T.temp=e),type:"number",style:{width:"182px"}},null,8,["modelValue"])])),_:1}),p(d,null,{default:c((()=>[p(C,{type:"primary",onClick:a[2]||(a[2]=e=>(e=>{if(!e)return;if(null===T.temp||""===T.temp)return void s.msgError("请输入温度");let a={mode:T.mode,temp:Number(T.temp)};l(a).then((e=>{s.msgSuccess("设置成功")})).catch((e=>{}))})(R.value))},{default:c((()=>[y("设置")])),_:1}),p(C,{onClick:a[3]||(a[3]=e=>{var a;(a=R.value)&&a.resetFields()})},{default:c((()=>[y("清空")])),_:1})])),_:1})])),_:1},8,["model"])])])),_:1})])])])),_:1}),p(t,{span:12},{default:c((()=>[f("div",M,[p(Z,{data:w.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:c((()=>[p(Y,{type:"index",width:"60",align:"center",label:"序号"}),p(Y,{prop:"name",label:"数据名称",align:"center"}),p(Y,{prop:"value",label:"数值",align:"center"}),p(Y,{prop:"unit",label:"单位",align:"center",width:"80"})])),_:1},8,["data"])])])),_:1})])),_:1})])}}}),[["__scopeId","data-v-708948fe"]]);export{N as default};