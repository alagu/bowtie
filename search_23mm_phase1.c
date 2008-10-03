/*
 * This is a fragment, included from multiple places in ebwt_search.cpp.
 * It implements the logic of the first phase of the 2/3-mismatch search
 * routine.  It is implemented as a code fragment so that it can be
 * reused in both the half-index-in-memory and full-index-in-memory
 * situations.
 */
{
	// If requested, check that this read has the same length
	// as all the previous ones
	params.setFw(true);
	params.setEbwtFw(true);
	if(plen < 3 && two) {
		cerr << "Error: Read (" << name << ") is less than 3 characters long" << endl;
		exit(1);
	}
	else if(plen < 4) {
		cerr << "Error: Read (" << name << ") is less than 4 characters long" << endl;
		exit(1);
	}
	bool hit;
	// Do an exact-match search on the forward pattern, just in
	// case we can pick it off early here
	ASSERT_ONLY(uint64_t numHits = sink.numHits());
	btr1.setQuery(&patFw, &qualFw, &name);
	btr1.setOffs(0, 0, plen, plen, plen, plen);
	hit = btr1.backtrack();
	assert(hit  || numHits == sink.numHits());
	assert(!hit || numHits <  sink.numHits());
	if(hit) {
		assert_eq(numHits+1, sink.numHits());
		DONEMASK_SET(patid);
		continue;
	}
	// Set up backtracker with reverse complement
	params.setFw(false);
	btr1.setQuery(&patRc, &qualRc, &name);
	// Set up the revisitability of the halves
	btr1.setOffs(0, 0, s5, s5, two ? s : s5, s);
	ASSERT_ONLY(numHits = sink.numHits());
	hit = btr1.backtrack();
	assert(hit  || numHits == sink.numHits());
	assert(!hit || numHits <  sink.numHits());
	if(hit) {
		DONEMASK_SET(patid);
		continue;
	}
}
