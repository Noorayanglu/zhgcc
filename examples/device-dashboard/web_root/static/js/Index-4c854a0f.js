import{_ as e,c as a}from"./dashboard-06d9dc70.js";import{l,m as t}from"./user-94f73423.js";import{j as s}from"./index-468025a3.js";import{g as r}from"./utils-e10d97cb.js";import{f as o}from"./feedback-5363c0cc.js";import{d as u,i as n,Y as i,e as d,Z as m,ag as p,c,a as v,F as f,a7 as b,u as j,U as x,O as y,az as _,aA as h,o as w,I as g,J as C,T as k,S,M as z}from"./@vue-c5a747fc.js";import{_ as B}from"./index-d54069a8.js";import"./lodash-20cd73ca.js";import"./dayjs-9ee440cb.js";import"./axios-5b55c11d.js";import"./element-plus-230cec57.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";import"./vue-router-641e505b.js";import"./pinia-801b3705.js";/* empty css                    */const A=[{name:"从机",value:0},{name:"恒功率放空",value:1},{name:"恒功率充满",value:2}],V=e=>(_("data-v-d3f8ed87"),e=e(),h(),e),I={class:"control"},M={class:"control-box"},E={class:"ctndata-box-box"},O=V((()=>v("div",{class:"ctndata-img"},[v("img",{src:e,alt:""})],-1))),P={class:"ctndata-box"},U={class:"ctn-name"},F={class:"ctn-value-box"},N=[V((()=>v("div",{class:"line-level"},null,-1)))],R={class:"btn-box"},D={class:"card-box"},G=V((()=>v("div",{class:"card-header"},[v("span",null,"控制")],-1))),J={class:"newpower-box"},T=V((()=>v("div",{class:"newpower-text"},"正值：放电 负值：充电",-1))),W={class:"newpower-box"},Y={class:"btn-box-new"},Z={class:"tab-box"},q=B(u({__name:"Index",setup(e){n((()=>{ie()})),i((()=>{ue()}));const u=A,_=a,h=d({}),B=d([]),V=d([]),q=d({}),H=d([]),K=d({}),L=d([]),Q=d({}),X=d([]),$=d({}),ee=d([]),ae=d(null),le=d(2e3),te=d(),se=m({param:null}),re=d(),oe=m({param:null}),ue=()=>{clearInterval(ae.value),ae.value=null},ne=e=>{t({cmd:e,param:0}).then((e=>{o.msgSuccess("设置成功")})).catch((e=>{}))},ie=()=>{l().then((e=>{h.value=e,B.value=me(h.value),V.value=de(h.value),ue(),ae.value=setInterval((()=>{ie()}),le.value)})).catch((e=>{}))},de=e=>{let a={"关口表通信状态":e["关口表通信状态"],"变压器表通信状态":e["变压器表通信状态"]};return s(a)},me=e=>{$.value={"储能状态":e["储能状态"],"工作模式":e["储能工作模式"],"故障信息":e["故障信息"],"储能功率":e["储能功率"],"关口功率":e["关口功率"],"光伏功率":e["光伏功率"],"负载功率":e["负载功率"],"变压器功率":e["变压器功率"]},K.value={"PCS状态":e["PCS状态"],"有功功率":e["PCS功率"],"AB线电压":e["AB线电压"],"BC线电压":e["BC线电压"],"CA线电压":e["CA线电压"]},q.value={SOC:e["BMS SOC"],"总电压":e["BMS总电压"],"总电流":e["BMS总电流"],"最高电芯电压":e["最高电芯电压"],"平均电芯电压":e["平均电芯电压"],"最低电芯电压":e["最低电芯电压"],"最高电芯温度":e["最高电芯温度"],"平均电芯温度":e["平均电芯温度"],"最低电芯温度":e["最低电芯温度"]},Q.value={"工作模式":e["空调工作模式"],"出水温度":e["出水温度"],"回水温度":e["回水温度"],"出水压力":e["出水压力"],"回水压力":e["回水压力"]},ee.value=s($.value),ee.value.map((e=>(e.value=r(e.value),e))),L.value=s(K.value),L.value.map((e=>(e.value=r(e.value),e))),H.value=s(q.value),H.value.map((e=>(e.value=r(e.value),e))),X.value=s(Q.value),X.value.map((e=>(e.value=r(e.value),e)));let a=[];return a.push(X.value,L.value,H.value,ee.value),a};return(e,a)=>{const l=p("el-input"),s=p("el-button"),r=p("el-form-item"),n=p("el-form"),i=p("el-option"),d=p("el-select"),m=p("el-card"),h=p("el-col"),A=p("el-table-column"),q=p("el-table"),H=p("el-row");return w(),c("div",I,[v("div",M,[v("div",E,[O,v("div",P,[(w(!0),c(f,null,b(B.value,((e,a)=>(w(),c("div",{key:a,class:g(["ctn-box",["ctn-box","ctn-box"+(a+1)]]),style:C("top: ".concat(j(_)[a].coordinate[0],"%; left: ").concat(j(_)[a].coordinate[1],"%;"))},[v("div",U,k(j(_)[a].name),1),v("div",F,[(w(!0),c(f,null,b(e,((e,a)=>(w(),c("div",{key:a+"A",class:"ctn-value"},[S(k(e.name)+"： ",1),v("span",null,k(e.value),1),S(" "+k(e.unit),1)])))),128))])],6)))),128)),(w(!0),c(f,null,b(j(_),((e,a)=>(w(),c("div",{key:a,class:g(["line-box","line-direction"+e.direction]),style:C("top: ".concat(e.coordinateline[0],"%; left: ").concat(e.coordinateline[1],"%;"))},N,6)))),128))])])]),v("div",R,[x(H,null,{default:y((()=>[x(h,{span:12,class:"col-box"},{default:y((()=>[v("div",D,[x(m,{class:"box-card"},{header:y((()=>[G])),default:y((()=>[v("div",null,[x(n,{ref_key:"formRef",ref:te,model:se,size:"default"},{default:y((()=>[x(r,{label:"功率设定",prop:"param"},{default:y((()=>[v("div",J,[x(l,{modelValue:se.param,"onUpdate:modelValue":a[0]||(a[0]=e=>se.param=e),type:"number",class:"newpower-input",style:{width:"190px"}},{append:y((()=>[S("kW")])),_:1},8,["modelValue"]),x(s,{class:"newpower-btn",type:"primary",onClick:a[1]||(a[1]=e=>(e=>{if(!e)return;if(null===se.param)return void o.msgError("请输入功率");let a={cmd:4,param:Number(se.param)};t(a).then((e=>{o.msgSuccess("设置成功")})).catch((e=>{}))})(te.value))},{default:y((()=>[S("设定")])),_:1})]),T])),_:1})])),_:1},8,["model"]),x(n,{ref_key:"formRef1",ref:re,model:oe,size:"default"},{default:y((()=>[x(r,{label:"工作模式",prop:"param"},{default:y((()=>[v("div",W,[x(d,{modelValue:oe.param,"onUpdate:modelValue":a[2]||(a[2]=e=>oe.param=e),placeholder:"工作模式",style:{width:"190px"}},{default:y((()=>[(w(!0),c(f,null,b(j(u),((e,a)=>(w(),z(i,{key:a,label:e.name,value:e.value},null,8,["label","value"])))),128))])),_:1},8,["modelValue"]),x(s,{class:"newpower-btn",type:"primary",onClick:a[3]||(a[3]=e=>(e=>{if(!e)return;if(null===oe.param)return void o.msgError("请选择工作模式");let a={cmd:5,param:Number(oe.param)};t(a).then((e=>{o.msgSuccess("设置成功")})).catch((e=>{}))})(re.value))},{default:y((()=>[S("设定")])),_:1})])])),_:1})])),_:1},8,["model"]),v("div",Y,[v("div",null,[x(s,{type:"primary",size:"default",onClick:a[4]||(a[4]=e=>ne(1))},{default:y((()=>[S("监控")])),_:1}),x(s,{type:"primary",size:"default",onClick:a[5]||(a[5]=e=>ne(2))},{default:y((()=>[S("停机")])),_:1}),x(s,{type:"primary",size:"default",onClick:a[6]||(a[6]=e=>ne(3))},{default:y((()=>[S("运行")])),_:1})])])])])),_:1})])])),_:1}),x(h,{span:12},{default:y((()=>[v("div",Z,[x(q,{data:V.value,stripe:"",border:"",align:"center",style:{width:"100%"}},{default:y((()=>[x(A,{type:"index",width:"60",align:"center",label:"序号"}),x(A,{prop:"name",label:"数据名称",align:"center"}),x(A,{prop:"value",label:"数值",align:"center"}),x(A,{prop:"unit",label:"单位",align:"center",width:"60"})])),_:1},8,["data"])])])),_:1})])),_:1})])])}}}),[["__scopeId","data-v-d3f8ed87"]]);export{q as default};
