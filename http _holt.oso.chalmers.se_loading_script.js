/* javascript for loading.html				
 *                                                     
 * Don't compare with null (==null is unpredictable)   
 * Declare all variables (otherwise strange behaviour) 
 * != operator does not exists (Javascript is not C...)
 *                                                    
 * August   2001  Machiel Bos			
 * 19 May   2006  Machiel Bos, allow no HARPOS for gravity
 * 9/6/2010       Machiel Bos, CIIMAR Porto
 * Jan 24,  2014  HGS,         stricter on pattern test
 *                             and a column test: blanks in 25 and 42 must have. 
 */
//==============================================================================
function sleep(ms)
{
    var dt = new Date();
    dt.setTime(dt.getTime() + ms);
    while (new Date().getTime() < dt.getTime());
}


function validate(form)
{
    var r,i,j,nlines,linelist,name,longitude,latitude,height,station,vs,nc,str;
    var ans,colchck,err,empty,emptyc;
    var pattern      = /^([^:|^\/|^\@]{24})\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)/;
    var patternshort = /^([^:|^\/|^\@]{24})\s+(-?\d+\.?\d*)\s+(-?\d+\.?\d*)/;
    var emptyline = /^\s*$/;
    var emptycol = /^ /;
    var comment = /^\/\//;
    var commas = /.*,.*/;
    var columns = /^(.){24}\s(.){14,16}\s(.)+/;
    var tabs = /\t/;
    
    //--- Exclude domains which cause SPAM trouble
    if ((form.MyEmail.value.indexOf("@yahoo") > -1) ||
        (form.MyEmail.value.indexOf("@homeworking") > -1) ||
        (form.MyEmail.value.indexOf("@hotmail") > -1 )) {
	alert("We do not serve users of this Internet domain - sorry");
	return false;
    }
    if (form.MyEmail.value.indexOf("@") < 1 ) {
	alert("This is not a valid email address");
        return false;
    }
    if (form.MyEmail.value.indexOf(".") < 1 ) {
	alert("This is not a valid email address");
        return false;
    }
    if (form.MyEmail.value.indexOf("<") > -1 ) {
	alert("Address must not contain html tags: " + form.MyEmail.value );
        return false;
    }
    if (form.MyEmail.value.indexOf("secret3162@") > -1 ) {
	alert("Congratulations, secret3162, you finally made it!");
	alert("We grant you 10 seconds to celebrate.");
        sleep(10000);
    }
    
    //--- Cannot apply CMC to gravity loading
    if (form.LoadingType[1].checked && form.CMC[1].checked) {
	alert("The centre of mass corrections cannot be applied to GRAVITY ocean tide loading");
	return false;
    }
    
    //--- Don't have gravity loading with HARPOS output format.
    if (form.LoadingType[1].checked && form.OutputFormat[1].checked) {
	alert("Unfortunately we don't provide HARPOS output for GRAVITY ocean tide loading");
	return false;
    }
    
    //--- Split the input form into a set of lines
    linelist = form.Stations.value.split(/\n/);
    nlines   = linelist.length;
    
    //--- Parse stations
    nc=0;
    for (i=0;i<nlines;i++) {
	j=i+1;
	skip=comment.exec(linelist[i]);
	empty=emptyline.exec(linelist[i]);
	emptyc=emptycol.exec(linelist[i]);
	if ( emptyc ){ alert("Please move station name to beginning of line"); return false};
	if ( empty ){ alert("Please delete empty lines"); return false};
	if ( skip || empty ){ nc++ };
/*	alert("Emptyline: " + i + " ->" + empty + "<-");
 */
	if ( !empty ){
            //--- Check if the line contains comma's
/*          alert("Line " + i + linelist[i]);
 */
            colchck=columns.exec(linelist[i]);
	    if ( !colchck && !skip ){
		alert("Unacceptable input");
		return false;
            }
	    
            err=tabs.exec(linelist[i]);
            if (err) {
		alert("Remove tab(s) in line " + j);
		return false;
            }
	    
            //--- Check if we're not dealing with a comment line
            err=comment.exec(linelist[i]);
/*	    alert("Comment test: " + err);
 */
            if (!err) {
		station=pattern.exec(linelist[i]);
                err=commas.exec(linelist[i]);
		if ( err && !station ){
		    alert("Please remove commas, replace them with decimal points.");
                    return false;
		} 
		
		if (!station){
		    alert("Garbled");
		    return false;   
		}
		if (station) {
		    if (station[1].indexOf("http://") > -1 ){ 
			return false;
		    }
		    //--- Check if we're dealing with longitude, latitude input
		    if (station[2]>-181.0 && station[2]<360.0 && 
			station[3]>-91.0 && station[3]<91.0) {
			height = station[4];
			if (height<-1000 || height>10000) {
			    alert ("In line "+j+", weird height! h="+height);
			    return false;
			}
			//--- X, Y, Z input
		    } else {
			r = Math.sqrt(station[2]*station[2] + station[3]*station[3] + 
				      station[4]*station[4]);
			if (r < 6.3e6 || r > 7.0e6) { 
   			    alert ("In line "+j+", weird radius: " + r);
			    return false;
			}
		    }
		    //--- Only 2 numbers have been given, see if we need to set height=0
		  } else if (form.LoadingType[0].checked && !station) {
		      station=patternshort.exec(linelist[i]);
		      if (!station) {
			  alert("Error #1, Could not read: " + i + " " + linelist[i] + 
				"\n\nPlease remove Tabs, non-ASCII characters and make sure you use points in the numbers and not commas.");
			  return false;
		      } else {
			  //--- Check if we're dealing with longitude,latitude input
			  if (station[2]>-181.0 && station[2]<360.0 && 
			      station[3]>-91.0 && station[3]<91.0) {
			      station[4] = 0.0;
			  } else {
			      alert("Error #3, Z component is missing");
			      return false;
			  }
		      }
		  } 
/*           else {
                alert("Error #2, Could not read: " + linelist[i] + 
"\n\nPlease remove Tabs, non-ASCII characters and make sure you use points in the numbers and not commas.");
                return false;
             } 
*/
            } 
	}    //--- endif: not a blank line
    }     //--- endfor

    /*    alert("Count comments: "+ nc + " count stations " + nlines); */

    if ( nc >= nlines ){ 
	alert("This is not a job: No stations, only comments.");
	return false; 
    }
    
    vs=nlines-nc;
      
    //--- Arrived here, so all must be well
/*    alert("final  :" + station[1] + ", " + station[2] + ", " + station[3] +
          ", " + station[4]);
*/
    ans = false;
    if ( vs == 1 ){
	ans=confirm("1 station seems acceptable for the job. Really submit?");
	
    } else if ( vs > 100 ){
	alert("Not more than 100 stations please");
	
    } else {
   	ans=confirm(vs + " stations seem acceptable for the job. Really submit?");
    }
    if ( ! ans ){ return false; }
    str = "Confirm your email address ->" + form.MyEmail.value + "<-";
    return confirm(str);
}
