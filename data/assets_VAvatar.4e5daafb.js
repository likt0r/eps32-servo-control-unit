import{x as c,ag as _,ah as m,a5 as v,ai as w,X as p,p as V,I as A,n as P,q as C,m as g}from"./index.15a97bb8.js";import{k as T,d as L,B as $,m as k,o as z,q as B,r as I,h as b,F as E,a as F,i as q,v as O,t as H}from"./router.6fe14ff4.js";function N(n){let l=arguments.length>1&&arguments[1]!==void 0?arguments[1]:"div",a=arguments.length>2?arguments[2]:void 0;return c({name:a!=null?a:_(m(n.replace(/__/g,"-"))),props:{tag:{type:String,default:l}},setup(s,r){let{slots:o}=r;return()=>{var t;return v(s.tag,{class:n},(t=o.default)==null?void 0:t.call(o))}}})}function i(n){let l=arguments.length>1&&arguments[1]!==void 0?arguments[1]:"top center 0",a=arguments.length>2?arguments[2]:void 0;return c({name:n,props:{group:Boolean,hideOnLeave:Boolean,leaveAbsolute:Boolean,mode:{type:String,default:a},origin:{type:String,default:l}},setup(s,r){let{slots:o}=r;return()=>{const t=s.group?w:p;return v(t,{name:n,mode:s.mode,onBeforeEnter(e){e.style.transformOrigin=s.origin},onLeave(e){if(s.leaveAbsolute){const{offsetTop:u,offsetLeft:f,offsetWidth:y,offsetHeight:d}=e;e._transitionInitialStyles={position:e.style.position,top:e.style.top,left:e.style.left,width:e.style.width,height:e.style.height},e.style.position="absolute",e.style.top=`${u}px`,e.style.left=`${f}px`,e.style.width=`${y}px`,e.style.height=`${d}px`}s.hideOnLeave&&e.style.setProperty("display","none","important")},onAfterLeave(e){if(s.leaveAbsolute&&e!=null&&e._transitionInitialStyles){const{position:u,top:f,left:y,width:d,height:x}=e._transitionInitialStyles;delete e._transitionInitialStyles,e.style.position=u||"",e.style.top=f||"",e.style.left=y||"",e.style.width=d||"",e.style.height=x||""}}},o.default)}}})}function h(n,l){let a=arguments.length>2&&arguments[2]!==void 0?arguments[2]:"in-out";return c({name:n,props:{mode:{type:String,default:a}},setup(s,r){let{slots:o}=r;return()=>v(p,{name:n,...l},o.default)}})}function S(){let n=arguments.length>0&&arguments[0]!==void 0?arguments[0]:"";const a=(arguments.length>1&&arguments[1]!==void 0?arguments[1]:!1)?"width":"height",s=m(`offset-${a}`);return{onBeforeEnter(t){t._parent=t.parentNode,t._initialStyle={transition:t.style.transition,overflow:t.style.overflow,[a]:t.style[a]}},onEnter(t){const e=t._initialStyle;t.style.setProperty("transition","none","important"),t.style.overflow="hidden";const u=`${t[s]}px`;t.style[a]="0",t.offsetHeight,t.style.transition=e.transition,n&&t._parent&&t._parent.classList.add(n),requestAnimationFrame(()=>{t.style[a]=u})},onAfterEnter:o,onEnterCancelled:o,onLeave(t){t._initialStyle={transition:"",overflow:t.style.overflow,[a]:t.style[a]},t.style.overflow="hidden",t.style[a]=`${t[s]}px`,t.offsetHeight,requestAnimationFrame(()=>t.style[a]="0")},onAfterLeave:r,onLeaveCancelled:r};function r(t){n&&t._parent&&t._parent.classList.remove(n),o(t)}function o(t){const e=t._initialStyle[a];t.style.overflow=t._initialStyle.overflow,e!=null&&(t.style[a]=e),delete t._initialStyle}}i("fab-transition","center center","out-in");i("dialog-bottom-transition");i("dialog-top-transition");i("fade-transition");i("scale-transition");i("scroll-x-transition");i("scroll-x-reverse-transition");i("scroll-y-transition");i("scroll-y-reverse-transition");i("slide-x-transition");i("slide-x-reverse-transition");const J=i("slide-y-transition");i("slide-y-reverse-transition");const W=h("expand-transition",S());h("expand-x-transition",S("",!0));const R=V({start:Boolean,end:Boolean,icon:A,image:String,...T(),...L(),...$(),...k(),...P(),...z({variant:"flat"})},"v-avatar"),X=c({name:"VAvatar",props:R(),setup(n,l){let{slots:a}=l;const{themeClasses:s}=C(n),{colorClasses:r,colorStyles:o,variantClasses:t}=B(n),{densityClasses:e}=I(n),{roundedClasses:u}=b(n),{sizeClasses:f,sizeStyles:y}=E(n);return F(()=>{var d;return g(n.tag,{class:["v-avatar",{"v-avatar--start":n.start,"v-avatar--end":n.end},s.value,r.value,e.value,u.value,f.value,t.value],style:[o.value,y.value]},{default:()=>[n.image?g(q,{key:"image",src:n.image,alt:"",cover:!0},null):n.icon?g(O,{key:"icon",icon:n.icon},null):(d=a.default)==null?void 0:d.call(a),H(!1,"v-avatar")]})}),{}}});export{W as V,X as a,J as b,N as c};
