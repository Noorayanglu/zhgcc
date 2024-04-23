import{u as e,b as l}from"./vue-router-641e505b.js";import{u as a,_ as s}from"./index-e7c70efc.js";import{d as t,e as n,j as i,f as o,ag as r,c as u,U as d,O as c,o as p,u as m,R as f,F as v,a7 as g,M as y,T as x,a as _,J as h,S as k,az as j,aA as b}from"./@vue-c5a747fc.js";import"./pinia-801b3705.js";import"./element-plus-230cec57.js";import"./lodash-es-fb3d0246.js";import"./@vueuse-90035fc5.js";import"./@element-plus-388117bf.js";import"./@popperjs-b78c3215.js";import"./@ctrl-91de2ec7.js";import"./dayjs-9ee440cb.js";import"./async-validator-cf877c1f.js";import"./memoize-one-63ab667a.js";import"./escape-html-92a447fa.js";import"./normalize-wheel-es-3222b0a2.js";import"./@floating-ui-9ca8b935.js";/* empty css                    */const w={class:"admin"},z={key:0,class:"logo"},F={key:0,class:"logoname"},C={key:1,class:"logo"},I={key:0,style:{"padding-left":"16px"}},L={style:{"padding-left":"16px"}},M={key:0,style:{"padding-left":"16px"}},T={class:"display-flex ai-center jc-space-between flex-1 pl-20"},$={class:"display-flex ai-center ac-center"},A=(e=>(j("data-v-3b1207d6"),e=e(),b(),e))((()=>_("div",null,"多棱多智慧工商业储能电站",-1))),E=["onClick"],J={class:"display-flex ai-center user"},O={class:"nav-menu-item"},P={class:"display-flex ai-center"},R={class:"el-dropdown-link cursor-pointer ml-6 fs-14"},S={style:{background:"#fff",padding:"20px"}},U=s(t({__name:"Index",setup(s){const t=a(),j=e();l();const b=n(!1),U=n(!0),q=i((()=>t.menu||[])),B=i((()=>t.breadcrumb||[]));o(b,(e=>{U.value=!U.value,setTimeout((()=>{U.value=!U.value}),100)}));const D=()=>{t.logout()};return(e,l)=>{const a=r("el-image"),s=r("svg-icon"),t=r("el-menu-item"),n=r("el-menu-item-group"),i=r("el-sub-menu"),o=r("el-menu"),G=r("el-aside"),H=r("Expand"),K=r("Fold"),N=r("el-icon"),Q=r("el-breadcrumb-item"),V=r("el-breadcrumb"),W=r("Monitor"),X=r("router-link"),Y=r("arrow-down"),Z=r("el-dropdown-item"),ee=r("el-dropdown-menu"),le=r("el-dropdown"),ae=r("el-header"),se=r("router-view"),te=r("el-scrollbar"),ne=r("el-main"),ie=r("el-container");return p(),u("div",w,[d(ie,{class:"height-percent-100"},{default:c((()=>[d(G,{width:b.value?"65px":"220px",class:"aside scrollbar"},{default:c((()=>[U.value?(p(),u("div",z,[d(a,{style:{width:"23px"},src:m("/static/png/logonew-af380e46.png")},null,8,["src"]),b.value?f("",!0):(p(),u("span",F,"正辉智慧终端"))])):(p(),u("div",C)),d(o,{"default-active":e.$route.path,collapse:b.value,class:"menu","text-color":"rgba(255, 255, 255, .9)",router:"","collapse-transition":!1,"active-text-color":"#1990FF","background-color":"rgba(38, 52, 69, .9)"},{default:c((()=>[(p(!0),u(v,null,g(q.value,((e,l)=>(p(),u(v,null,[e.childList.length?(p(),y(i,{key:l,index:e.url},{title:c((()=>[d(s,{name:e.icon},null,8,["name"]),b.value?f("",!0):(p(),u("span",I,x(e.name),1))])),default:c((()=>[d(n,null,{default:c((()=>[(p(!0),u(v,null,g(e.childList,((e,l)=>(p(),y(t,{key:l,index:e.url},{title:c((()=>[d(s,{name:e.icon},null,8,["name"]),_("span",L,x(e.name),1)])),_:2},1032,["index"])))),128))])),_:2},1024)])),_:2},1032,["index"])):(p(),y(t,{key:l+1,index:e.url},{default:c((()=>[d(s,{name:e.icon},null,8,["name"]),b.value?f("",!0):(p(),u("div",M,x(e.name),1))])),_:2},1032,["index"]))],64)))),256))])),_:1},8,["default-active","collapse"])])),_:1},8,["width"]),d(ie,null,{default:c((()=>[d(ae,{class:"display-flex fd-column jc-center header"},{default:c((()=>[_("div",T,[_("div",$,[d(N,{class:"mr-12",size:24,onClick:l[0]||(l[0]=e=>b.value=!b.value)},{default:c((()=>[b.value?(p(),y(H,{key:0})):(p(),y(K,{key:1}))])),_:1}),A,d(V,{"separator-class":"el-icon-arrow-right"},{default:c((()=>[(p(!0),u(v,null,g(B.value,((e,l)=>(p(),y(Q,{key:l},{default:c((()=>[_("span",{class:"cursor-pointer",style:h({color:B.value.length-1===l?"#1990FF":"#999999"}),onClick:e=>(e=>{e+1!==B.value.length&&0!==e&&j.go(e+1-B.value.length)})(l)},x(e),13,E)])),_:2},1024)))),128))])),_:1})]),_("div",J,[d(X,{class:"h100",target:"_blank",title:"首页",to:"/"},{default:c((()=>[_("div",O,[d(N,{color:"#000",style:{"font-size":"20px"}},{default:c((()=>[d(W)])),_:1})])])),_:1}),d(le,{size:"large",trigger:"click"},{dropdown:c((()=>[d(ee,null,{default:c((()=>[d(Z,{onClick:D},{default:c((()=>[k(" 退出登录 ")])),_:1})])),_:1})])),default:c((()=>[_("div",P,[_("span",R,[k(" 管理员"),d(N,{class:"el-icon--right"},{default:c((()=>[d(Y)])),_:1})])])])),_:1})])])])),_:1}),d(ne,{class:"scrollbar-y",style:{height:"calc(100vh - 140px)",background:"#f4f4f4",padding:"0"}},{default:c((()=>[d(te,null,{default:c((()=>[_("div",S,[(p(),y(se,{key:e.$route.fullPath}))])])),_:1})])),_:1})])),_:1})])),_:1})])}}}),[["__scopeId","data-v-3b1207d6"]]);export{U as default};
