var n=Object.defineProperty,t=(t,e,r)=>(((t,e,r)=>{e in t?n(t,e,{enumerable:!0,configurable:!0,writable:!0,value:r}):t[e]=r})(t,"symbol"!=typeof e?e+"":e,r),r);import{E as e,a as r,b as s,c as a}from"./element-plus-a5ae5727.js";const c=class n{constructor(){t(this,"loadingInstance",null)}static getInstance(){var t;return null!=(t=this.instance)?t:this.instance=new n}msg(n){e.info(n)}msgError(n){e.error(n)}msgSuccess(n){e.success(n)}msgWarning(n){e.warning(n)}alert(n){r.alert(n,"系统提示")}alertError(n){r.alert(n,"系统提示",{type:"error"})}alertSuccess(n){r.alert(n,"系统提示",{type:"success"})}alertWarning(n){r.alert(n,"系统提示",{type:"warning"})}notify(n){s.info(n)}notifyError(n){s.error(n)}notifySuccess(n){s.success(n)}notifyWarning(n){s.warning(n)}confirm(n){return r.confirm(n,"温馨提示",{confirmButtonText:"确定",cancelButtonText:"取消",type:"warning"})}prompt(n,t,e){return r.prompt(n,t,{confirmButtonText:"确定",cancelButtonText:"取消",...e})}loading(n){this.loadingInstance=a.service({lock:!0,text:n})}closeLoading(){var n;null==(n=this.loadingInstance)||n.close()}};t(c,"instance",null);const o=c.getInstance();export{o as f};
