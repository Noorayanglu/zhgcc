import{i as e,j as a,k as l}from"./index-64e13220.js";import{f as s}from"./feedback-551b2bff.js";import{d as t,e as n,Z as r,i as o,Y as u,ag as i,c as d,U as c,O as m,o as p,a as v,F as f,a7 as b,S as j,M as g,az as x,aA as h}from"./@vue-c5a747fc.js";import{_}from"./index-0c73d0ca.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-a5ae5727.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const k=e=>(x("data-v-b12875ec"),e=e(),h(),e),w={class:"interface"},y=k((()=>v("div",{class:"img-box"},[v("img",{src:"/static/png/gkj1-442398ae.png",alt:""})],-1))),z=k((()=>v("div",{class:"img-box"},[v("img",{src:"/static/png/gkj2-de611663.png",alt:""})],-1))),I={class:"btn-box"},V={class:"card-box"},A=k((()=>v("div",{class:"card-header"},[v("span",null,"控制")],-1))),M={class:"newpower-box"},O={class:"ctnbox-box"},S={class:"ctn-box"},U=_(t({__name:"Index",setup(t){const x=n(),h=r({index:null});n([]);const _=n({}),k=n([]),U=n([]),B=n([]);n([]);const C=n(null),E=n(2e3);o((()=>{R()})),u((()=>{F()}));const F=()=>{clearInterval(C.value),C.value=null},N=e=>{l({cmd:1,index:e}).then((e=>{s.msgSuccess("复位成功")})).catch((e=>{}))},R=()=>{e().then((e=>{_.value=e,k.value=[],U.value=[],a(_.value).map(((e,a)=>{e.name.indexOf("485")>-1?k.value.push(e):U.value.push(e)})),k.value.map(((e,l)=>(e.value=a(e.value),e.value.map((a=>(a[e.name]=a.value,a))),e))),k.value.reduce(((e,a,l)=>{let s=((e,a,l)=>{const s=new Map;for(const t of e.concat(a))if(s.has(t[l])){const e=s.get(t[l]);for(const a in t)a!==l&&(e[a]=t[a])}else s.set(t[l],t);return Array.from(s.values())})(e.value,a.value,"name");return l===k.value.length-1&&(B.value=s),{value:s}})),U.value.map(((e,l)=>(e.value=a(e.value),e))),F(),C.value=setInterval((()=>{R()}),E.value)})).catch((e=>{}))};return(e,a)=>{const l=i("el-option"),t=i("el-select"),n=i("el-button"),r=i("el-form-item"),o=i("el-form"),u=i("el-card"),_=i("el-col"),C=i("el-table-column"),E=i("el-table"),F=i("el-row");return p(),d("div",w,[c(F,null,{default:m((()=>[c(_,{span:8},{default:m((()=>[y,z,v("div",I,[v("div",V,[c(u,{class:"box-card"},{header:m((()=>[A])),default:m((()=>[v("div",null,[c(o,{ref_key:"formRef",ref:x,model:h,size:"default"},{default:m((()=>[c(r,{label:"通道选择",prop:"index"},{default:m((()=>[v("div",M,[c(t,{modelValue:h.index,"onUpdate:modelValue":a[0]||(a[0]=e=>h.index=e),placeholder:"通道",class:"newpower-input"},{default:m((()=>[(p(),d(f,null,b(6,(e=>c(l,{key:e,label:e,value:e},null,8,["label","value"]))),64))])),_:1},8,["modelValue"]),c(n,{type:"primary",onClick:a[1]||(a[1]=e=>{x.value&&(null!==h.index?N(Number(h.index)):s.msgError("请选择通道"))})},{default:m((()=>[j("复位")])),_:1})])])),_:1})])),_:1},8,["model"])])])),_:1})])])])),_:1}),c(_,{span:16},{default:m((()=>[v("div",O,[v("div",S,[c(E,{data:B.value,stripe:"",border:"",align:"center"},{default:m((()=>[c(C,{prop:"name",label:"数据名称",align:"center"}),(p(!0),d(f,null,b(k.value,((e,a)=>(p(),g(C,{key:a,prop:e.name,label:e.name,align:"center","min-width":"100"},null,8,["prop","label"])))),128))])),_:1},8,["data"])]),(p(!0),d(f,null,b(U.value,((e,a)=>(p(),d("div",{key:a,class:"ctn-box"},[c(E,{data:e.value,stripe:"",border:"",align:"center"},{default:m((()=>[c(C,{label:e.name,align:"center"},{default:m((()=>[c(C,{prop:"name",label:"数据名称",align:"center"}),c(C,{prop:"value",label:"数值",align:"center","min-width":"130"})])),_:2},1032,["label"])])),_:2},1032,["data"])])))),128))])])),_:1})])),_:1})])}}}),[["__scopeId","data-v-b12875ec"]]);export{U as default};
